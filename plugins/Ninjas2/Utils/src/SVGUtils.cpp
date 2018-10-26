#include "src/DistrhoDefines.h"
#include "DistrhoUtils.hpp"

#define NANOSVG_ALL_COLOR_KEYWORDS // Include full list of color keywords.
#define NANOSVG_IMPLEMENTATION     // Expands implementation
#include "SVGUtils.hpp"

START_NAMESPACE_DISTRHO

namespace SVGUtils
{

//based on VCVRack's code: https://raw.githubusercontent.com/VCVRack/Rack/v0.6/src/widgets/SVGWidget.cpp

NVGcolor getNVGColor(uint32_t color)
{
    return nvgRGBA(
        (color >> 0) & 0xff,
        (color >> 8) & 0xff,
        (color >> 16) & 0xff,
        (color >> 24) & 0xff);
}

NVGpaint getPaint(NVGcontext *vg, NSVGpaint *p)
{
    assert(p->type == NSVG_PAINT_LINEAR_GRADIENT || p->type == NSVG_PAINT_RADIAL_GRADIENT);
    NSVGgradient *g = p->gradient;
    assert(g->nstops >= 1);
    NVGcolor icol = getNVGColor(g->stops[0].color);
    NVGcolor ocol = getNVGColor(g->stops[g->nstops - 1].color);

    float inverse[6];
    nvgTransformInverse(inverse, g->xform);
    Vec s, e;
    // Is it always the case that the gradient should be transformed from (0, 0) to (0, 1)?
    nvgTransformPoint(&s.x, &s.y, inverse, 0, 0);
    nvgTransformPoint(&e.x, &e.y, inverse, 0, 1);

    NVGpaint paint;
    if (p->type == NSVG_PAINT_LINEAR_GRADIENT)
        paint = nvgLinearGradient(vg, s.x, s.y, e.x, e.y, icol, ocol);
    else
        paint = nvgRadialGradient(vg, s.x, s.y, 0.0, 160, icol, ocol);
    return paint;
}

/** Returns the parameterized value of the line p2--p3 where it intersects with p0--p1 */
float getLineCrossing(Vec p0, Vec p1, Vec p2, Vec p3) {
	Vec b = p2.minus(p0);
	Vec d = p1.minus(p0);
	Vec e = p3.minus(p2);
	float m = d.x * e.y - d.y * e.x;
	// Check if lines are parallel, or if either pair of points are equal
	if (fabsf(m) < 1e-6)
		return NAN;
	return -(d.x * b.y - d.y * b.x) / m;
}

void nvgDrawSVG(NVGcontext *vg, NSVGimage *svg)
{
    DISTRHO_SAFE_ASSERT(svg != nullptr)

    int shapeIndex = 0;

    for (NSVGshape *shape = svg->shapes; shape; shape = shape->next, shapeIndex++)
    {
        if (!(shape->flags & NSVG_FLAGS_VISIBLE))
            continue;

        nvgSave(vg);

        if (shape->opacity < 1.0)
            nvgGlobalAlpha(vg, shape->opacity);

        nvgBeginPath(vg);

        for (NSVGpath *path = shape->paths; path; path = path->next)
        {
            nvgMoveTo(vg, path->pts[0], path->pts[1]);

            for (int i = 1; i < path->npts; i += 3)
            {
                float *p = &path->pts[2 * i];
                nvgBezierTo(vg, p[0], p[1], p[2], p[3], p[4], p[5]);
            }

            if (path->closed)
                nvgClosePath(vg);

            // Compute whether this is a hole or a solid.
            // Assume that no paths are crossing (usually true for normal SVG graphics).
            // Also assume that the topology is the same if we use straight lines rather than Beziers (not always the case but usually true).
            // Using the even-odd fill rule, if we draw a line from a point on the path to a point outside the boundary (e.g. top left) and count the number of times it crosses another path, the parity of this count determines whether the path is a hole (odd) or solid (even).
            int crossings = 0;

            Vec p0 = Vec(path->pts[0], path->pts[1]);
            Vec p1 = Vec(path->bounds[0] - 1.0, path->bounds[1] - 1.0);

            // Iterate all other paths
            for (NSVGpath *path2 = shape->paths; path2; path2 = path2->next)
            {
                if (path2 == path)
                    continue;

                // Iterate all lines on the path
                if (path2->npts < 4)
                    continue;

                for (int i = 1; i < path2->npts + 3; i += 3)
                {
                    float *p = &path2->pts[2 * i];

                    // The previous point
                    Vec p2 = Vec(p[-2], p[-1]);
                    // The current point
                    Vec p3 = (i < path2->npts) ? Vec(p[4], p[5]) : Vec(path2->pts[0], path2->pts[1]);

                    float crossing = getLineCrossing(p0, p1, p2, p3);
                    float crossing2 = getLineCrossing(p2, p3, p0, p1);

                    if (0.0 <= crossing && crossing < 1.0 && 0.0 <= crossing2)
                    {
                        crossings++;
                    }
                }
            }

            if (crossings % 2 == 0)
                nvgPathWinding(vg, NVG_SOLID);
            else
                nvgPathWinding(vg, NVG_HOLE);
        }

        // Fill shape
        if (shape->fill.type)
        {
            switch (shape->fill.type)
            {
            case NSVG_PAINT_COLOR:
            {
                NVGcolor color = getNVGColor(shape->fill.color);
                nvgFillColor(vg, color);
            }
            break;
            case NSVG_PAINT_LINEAR_GRADIENT:
            case NSVG_PAINT_RADIAL_GRADIENT:
            {
                NSVGgradient *g = shape->fill.gradient;
                (void)g;
                for (int i = 0; i < g->nstops; i++)
                {
                }

                nvgFillPaint(vg, getPaint(vg, &shape->fill));
            }
            break;
            }

            nvgFill(vg);
        }

        // Stroke shape
        if (shape->stroke.type)
        {
            nvgStrokeWidth(vg, shape->strokeWidth);
            nvgLineCap(vg, (NVGlineCap)shape->strokeLineCap);
            nvgLineJoin(vg, (int)shape->strokeLineJoin);

            switch (shape->stroke.type)
            {
            case NSVG_PAINT_COLOR:
            {
                NVGcolor color = getNVGColor(shape->stroke.color);
                nvgStrokeColor(vg, color);
            }
            break;
            case NSVG_PAINT_LINEAR_GRADIENT:
            {
            }
            break;
            }
            nvgStroke(vg);
        }

        nvgRestore(vg);
    }
}

}; // namespace SVGUtils

END_NAMESPACE_DISTRHO