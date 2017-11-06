#pragma once

#include <QMainWindow>
#include <QEvent>

class Event
{
public:
    explicit Event() {}

    void mouseMove(const QMouseEvent* mouse_event)
    {
    }

    void mousePress(const QMouseEvent* mouse_event)
    {
    }

    void keyPress(const QKeyEvent* key_event)
    {
    }
};
