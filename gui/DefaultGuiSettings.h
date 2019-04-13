//
// Created by md3s on 13.04.19.
//

#ifndef PATHFINDING_PROBLEM_STANDARDGUISETTINGS_H
#define PATHFINDING_PROBLEM_STANDARDGUISETTINGS_H

#include <QtGui/QColor>

// dgs = default gui setting
class dgs {
public:
    // Default map
    static const std::string defaultMap;

    // Default Camera
    static constexpr float zoom = 1,
          shiftX = 0,
          shiftY = 0,
          zoomAngle = 0,
          recentShiftX = 0,
          recentShiftY = 0,
          vertexRadius = 6;

    static constexpr int arrowShiftSpeed = 20,
        pixelRadiusClickAreaSearch = 100;

    static constexpr bool vertexHighlight = false,
         hlPath = true,
         hlForwardPath = true,
         hlReversePath = true,
         hlForwardEdges = true,
         hlReverseEdges = true;

    // Default Colors
    static const QColor vertexColor;
    static const QColor edgeColor;
    static const QColor selectedVertexColor;
    static const QColor selectedEdgeColor;
    static const QColor pathColor;
    static const QColor forwardSearchPathEdgeColor;
    static const QColor reverseSearchPathEdgeColor;
    static const QColor forwardSearchAreaColor;
    static const QColor reverseSearchAreaColor;
};

#endif //PATHFINDING_PROBLEM_STANDARDGUISETTINGS_H
