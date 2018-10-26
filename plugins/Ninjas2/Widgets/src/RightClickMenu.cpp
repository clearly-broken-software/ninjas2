#include "RightClickMenu.hpp"
#include <unistd.h>

START_NAMESPACE_DISTRHO

RightClickMenuItem::RightClickMenuItem(int id, const char *label, const char *comment, bool enabled) noexcept : fId(id),
                                                                                                                fLabel(label),
                                                                                                                fComment(comment),
                                                                                                                fEnabled(enabled),
                                                                                                                fIsSection(false)
{
}

int RightClickMenuItem::getId()
{
    return fId;
}

const char *RightClickMenuItem::getLabel()
{
    return fLabel;
}

const char *RightClickMenuItem::getComment()
{
    return fComment;
}

bool RightClickMenuItem::getEnabled()
{
    return fEnabled;
}

void RightClickMenuItem::setEnabled(bool enabled)
{
    fEnabled = enabled;
}

bool RightClickMenuItem::isSection()
{
    return fIsSection;
}

RightClickMenuSection::RightClickMenuSection(const char *label) noexcept : RightClickMenuItem(-1, label, "", false)
{
    fIsSection = true;
}

RightClickMenu::RightClickMenu(NanoWidget *parent) noexcept : Window(parent->getParentApp(), parent->getParentWindow()),
                                                              NanoWidget((Window &)*this),
                                                              fParent(parent),
                                                              fFontSize(17.0f),
                                                              fSectionFontSize(14.0f),
                                                              fBorderColor(Color(161, 67, 198, 255)),
                                                              fMargin(Margin(7, 15, 7, 13)),
                                                              fLongestWidth(0.0f)
{
    Window::setResizable(false);
    Window::setBorderless(true);

    loadSharedResources();
}

RightClickMenu::~RightClickMenu()
{
    Window::close();
}

void RightClickMenu::show(int posX, int posY)
{
    //we don't want the mouse to intersect with the popup straight away, so we add a bit of margin
    posX += 2;
    posY += 2;

    adaptSize();

    Window::hideFromTaskbar();

    Window &parentWindow = fParent->getParentWindow();
    Point<int> windowPos = parentWindow.getAbsolutePos();

    //FIXME: this is really a mess... right now, it's necessary to set the size before and after the exec to get the correct window dimensions on win32...
    //it still flickers a bit, so it's not a perfect solution
#if defined(DISTRHO_OS_WINDOWS)
    adaptSize();

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
#endif

    Window::exec(false);

#if defined(DISTRHO_OS_WINDOWS)
    adaptSize();
#endif

    Window::setAbsolutePos(posX + windowPos.getX(), posY + windowPos.getY());
}

void RightClickMenu::close()
{
    Window::close();
}

void RightClickMenu::setBorderColor(const Color color)
{
    fBorderColor = color;
}

void RightClickMenu::setRegularFontSize(float fontSize)
{
    fFontSize = fontSize;
}

void RightClickMenu::setSectionFontSize(float fontSize)
{
    fSectionFontSize = fontSize;
}

Rectangle<float> RightClickMenu::getBoundsOfItemComment(const int index)
{
    fontSize(fSectionFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;
    Rectangle<float> itemBounds = getBoundsOfItem(index);

    textBounds(itemBounds.getX() + itemBounds.getWidth(), index * fFontSize + fMargin.top, fItems[index].getComment(), NULL, bounds);

    return bounds;
}

Rectangle<float> RightClickMenu::getBoundsOfItem(const int index)
{
    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);

    Rectangle<float> bounds;

    textBounds(0 + fMargin.left, index * fFontSize + fMargin.top, fItems[index].getLabel(), NULL, bounds);

    return bounds;
}

void RightClickMenu::setSectionEnabled(int index, bool enabled)
{
    DISTRHO_SAFE_ASSERT(index >= 0)

    int sectionCount = -1;

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        if (fItems[i].isSection())
        {
            ++sectionCount;

            if (sectionCount == index)
            {
                ++i;

                while (i < fItems.size() && !fItems[i].isSection())
                {
                    fItems[i].setEnabled(enabled);
                    ++i;
                }

                return;
            }
        }
    }
}

void RightClickMenu::adaptSize()
{
    findLongestItem();

    const Size<uint> size = Size<uint>(fLongestWidth + fMargin.left + fMargin.right + 12, fItems.size() * fFontSize + fMargin.top + fMargin.bottom);

    Window::setSize(size);
    NanoWidget::setSize(size);
}

RightClickMenuItem *RightClickMenu::getItemById(int id)
{
    //TODO: optimize with binary search or something
    for (size_t i = 0; i < fItems.size(); ++i)
    {
        if (fItems[i].getId() == id)
        {
            return &fItems[i];
        }
    }

    return nullptr;
}

bool RightClickMenuItem::hasComment()
{
    return strcmp(fComment, "") != 0;
}

void RightClickMenu::findLongestItem()
{
    fLongestWidth = 0.0f;

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        float itemWidth = getBoundsOfItem(i).getWidth();

        if (fItems[i].hasComment())
        {
            itemWidth += getBoundsOfItemComment(i).getWidth();
        }

        if (itemWidth > fLongestWidth)
        {
            fLongestWidth = itemWidth;
        }
    }
}

void RightClickMenu::addSection(const char *sectionName)
{
    RightClickMenuSection section = RightClickMenuSection(sectionName);

    fItems.push_back(section);
}

void RightClickMenu::addItem(int id, const char *label, const char *comment)
{
    DISTRHO_SAFE_ASSERT(id >= 0)

    RightClickMenuItem item = RightClickMenuItem(id, label, comment, true);

    fItems.push_back(item);
}

void RightClickMenu::setCallback(Callback *callback) noexcept
{
    fCallback = callback;
}

void RightClickMenu::onNanoDisplay()
{
    const float width = NanoWidget::getWidth();
    const float height = NanoWidget::getHeight();

    beginPath();

    Rectangle<float> bounds;

    fontSize(fFontSize);
    textAlign(ALIGN_LEFT | ALIGN_TOP);
    textBounds(0, 0, fItems[0].getLabel(), NULL, bounds);

    fillColor(Color(39, 39, 39, 255));

    strokeColor(fBorderColor);
    strokeWidth(3.0f);

    rect(0, 0, width, height);
    fill();
    stroke();

    closePath();

    float verticalOffset = 0;

    translate(fMargin.left, fMargin.top);

    for (size_t i = 0; i < fItems.size(); ++i)
    {
        beginPath();

        if (fItems[i].isSection())
        {
            fontSize(fSectionFontSize);
        }
        else
        {
            fontSize(fFontSize);
        }

        if (fItems[i].getEnabled() == true)
        {
            fillColor(Color(255, 255, 255, 255));
        }
        else
        {
            fillColor(Color(100, 100, 100, 255));
        }

        text(fItems[i].isSection() ? 0 : 12, verticalOffset, fItems[i].getLabel(), NULL);
        
        if (fItems[i].hasComment())
        {            
            fontSize(fSectionFontSize); //FIXME: wrong font size?
            fillColor(Color(100, 100, 100, 255));

            text(getBoundsOfItem(i).getWidth() + 12 + 4, verticalOffset, fItems[i].getComment(), NULL);
        }

        verticalOffset += bounds.getHeight();

        closePath();
    }
}

bool RightClickMenu::onMouse(const MouseEvent &ev)
{
    if (ev.press == true)
    {
        for (size_t i = 0; i < fItems.size(); ++i)
        {
            if (fItems[i].getEnabled() == true && getBoundsOfItem(i).contains(Point<float>(ev.pos.getX(), ev.pos.getY())))
            {
                fCallback->rightClickMenuItemSelected(&fItems[i]);

                close();

                return true;
            }
        }
    }

    return true;
}

void RightClickMenu::onFocusOut()
{
    close();
}

END_NAMESPACE_DISTRHO
