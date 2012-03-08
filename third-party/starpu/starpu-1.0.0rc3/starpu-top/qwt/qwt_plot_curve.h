/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_CURVE_H
#define QWT_PLOT_CURVE_H

#include "qwt_global.h"
#include "qwt_plot_seriesitem.h"
#include "qwt_series_data.h"
#include "qwt_text.h"
#include <qpen.h>
#include <qstring.h>

class QPainter;
class QPolygonF;
class QwtScaleMap;
class QwtSymbol;
class QwtCurveFitter;

/*!
  \brief A plot item, that represents a series of points

  A curve is the representation of a series of points in the x-y plane.
  It supports different display styles, interpolation ( f.e. spline )
  and symbols.

  \par Usage
  <dl><dt>a) Assign curve properties</dt>
  <dd>When a curve is created, it is configured to draw black solid lines
  with in Lines style and no symbols. You can change this by calling
  setPen(), setStyle() and setSymbol().</dd>
  <dt>b) Connect/Assign data.</dt>
  <dd>QwtPlotCurve gets its points using a QwtData object offering
  a bridge to the real storage of the points ( like QAbstractItemModel ).
  There are several convenience classes derived from QwtData, that also store
  the points inside ( like QStandardItemModel ). QwtPlotCurve also offers
  a couple of variations of setData(), that build QwtData objects from
  arrays internally.</dd>
  <dt>c) Attach the curve to a plot</dt>
  <dd>See QwtPlotItem::attach()
  </dd></dl>

  \par Example:
  see examples/bode

  \sa QwtPointSeriesData, QwtSymbol, QwtScaleMap
*/
class QWT_EXPORT QwtPlotCurve: public QwtPlotSeriesItem<QPointF>
{
public:
    /*!
        Curve styles.

         - NoCurve\n
           Don't draw a curve. Note: This doesn't affect the symbols.

         - Lines\n
           Connect the points with straight lines. The lines might
           be interpolated depending on the 'Fitted' attribute. Curve
           fitting can be configured using setCurveFitter().

         - Sticks\n
           Draw vertical or horizontal sticks ( depending on the 
           orientation() ) from a baseline which is defined by setBaseline().

         - Steps\n
           Connect the points with a step function. The step function
           is drawn from the left to the right or vice versa,
           depending on the 'Inverted' attribute.

         - Dots\n
           Draw dots at the locations of the data points. Note:
           This is different from a dotted line (see setPen()), and faster
           as a curve in NoStyle style and a symbol painting a point.

         - UserCurve\n
           Styles >= UserCurve are reserved for derived
           classes of QwtPlotCurve that overload drawCurve() with
           additional application specific curve types.

        \sa setStyle(), style()
    */
    enum CurveStyle
    {
        NoCurve,

        Lines,
        Sticks,
        Steps,
        Dots,

        UserCurve = 100
    };

    /*!
      Attribute for drawing the curve

      - Fitted ( in combination with the Lines QwtPlotCurve::CurveStyle only )\n
        A QwtCurveFitter tries to
        interpolate/smooth the curve, before it is painted.
        Note that curve fitting requires temorary memory
        for calculating coefficients and additional points.
        If painting in Fitted mode is slow it might be better
        to fit the points, before they are passed to QwtPlotCurve.
      - Inverted\n
        For Steps only. Draws a step function
        from the right to the left.

        \sa setCurveAttribute(), testCurveAttribute(), curveFitter()
    */
    enum CurveAttribute
    {
        Inverted = 1,
        Fitted = 2
    };

    /*!
        Attributes how to represent the curve on the legend

        - LegendShowLine
          If the curveStyle() is not NoCurve a line is painted with the
          curvePen().
        - LegendShowSymbol
          If the curve has a valid symbol it is painted.
        - LegendShowBrush
          If the curve has a brush a rectangle filled with this brush
          is painted

        If none of the flags is activated QwtPlotCurve tries to find
        a color representing the curve and paints a rectangle with it.
        In the default setting all attributes are off.

        \sa setLegendAttribute(), testLegendAttribute(),
            drawLegendIdentifier()
     */

    enum LegendAttribute
    {
        LegendShowLine = 1,
        LegendShowSymbol = 2,
        LegendShowBrush = 4
    };

    /*!
        Attributes to modify the drawing algorithm.

        - ClipPolygons\n
          Clip polygons before painting them. In situations, where points
          are far outside the visible area (f.e when zooming deep) this
          might be a substantial improvement for the painting performance
          ( especially on Windows ).
        - CacheSymbols\n
          Paint the symbol to a QPixmap and paint the pixmap
          instead rendering the symbol for each point. The flag has
          no effect, when the curve is not painted to the canvas
          ( or its cache )

        The default setting enables ClipPolygons

        \sa setPaintAttribute(), testPaintAttribute()
    */
    enum PaintAttribute
    {
        ClipPolygons = 1,
        CacheSymbols = 2
    };

    explicit QwtPlotCurve( const QString &title = QString::null );
    explicit QwtPlotCurve( const QwtText &title );

    virtual ~QwtPlotCurve();

    virtual int rtti() const;

    void setPaintAttribute( PaintAttribute, bool on = true );
    bool testPaintAttribute( PaintAttribute ) const;

    void setLegendAttribute( LegendAttribute, bool on = true );
    bool testLegendAttribute( LegendAttribute ) const;

#ifndef QWT_NO_COMPAT
    void setRawSamples( const double *xData, const double *yData, int size );
    void setSamples( const double *xData, const double *yData, int size );
    void setSamples( const QVector<double> &xData, const QVector<double> &yData );
#endif
    void setSamples( const QVector<QPointF> & );

    int closestPoint( const QPoint &pos, double *dist = NULL ) const;

    double minXValue() const;
    double maxXValue() const;
    double minYValue() const;
    double maxYValue() const;

    void setCurveAttribute( CurveAttribute, bool on = true );
    bool testCurveAttribute( CurveAttribute ) const;

    void setPen( const QPen & );
    const QPen &pen() const;

    void setBrush( const QBrush & );
    const QBrush &brush() const;

    void setBaseline( double ref );
    double baseline() const;

    void setStyle( CurveStyle style );
    CurveStyle style() const;

    void setSymbol( const QwtSymbol *s );
    const QwtSymbol *symbol() const;

    void setCurveFitter( QwtCurveFitter * );
    QwtCurveFitter *curveFitter() const;

    virtual void drawSeries( QPainter *,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    virtual void updateLegend( QwtLegend * ) const;
    virtual void drawLegendIdentifier( QPainter *, const QRectF & ) const;

protected:

    void init();

    virtual void drawCurve( QPainter *p, int style,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    virtual void drawSymbols( QPainter *p, const QwtSymbol &,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    void drawLines( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    void drawSticks( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    void drawDots( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    void drawSteps( QPainter *p,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    virtual void fillCurve( QPainter *,
        const QwtScaleMap &, const QwtScaleMap &, QPolygonF & ) const;

    void closePolyline( QPainter *,
        const QwtScaleMap &, const QwtScaleMap &, QPolygonF & ) const;

private:
    class PrivateData;
    PrivateData *d_data;
};

//! boundingRect().left()
inline double QwtPlotCurve::minXValue() const
{
    return boundingRect().left();
}

//! boundingRect().right()
inline double QwtPlotCurve::maxXValue() const
{
    return boundingRect().right();
}

//! boundingRect().top()
inline double QwtPlotCurve::minYValue() const
{
    return boundingRect().top();
}

//! boundingRect().bottom()
inline double QwtPlotCurve::maxYValue() const
{
    return boundingRect().bottom();
}

#endif
