//
// begin license header
//
// This file is part of Pixy CMUcam5 or "Pixy" for short
//
// All Pixy source code is provided under the terms of the
// GNU General Public License v2 (http://www.gnu.org/licenses/gpl-2.0.html).
// Those wishing to use Pixy source code, software and/or
// technologies under different licensing terms should contact us at
// cmucam@cs.cmu.edu. Such licensing terms are available for
// all portions of the Pixy codebase presented here.
//
// end license header
//

#ifndef RENDERER_H
#define RENDERER_H
#include <QObject>
#include <QImage>
#include "pixytypes.h"
#include "processblobs.h"

class Interpreter;

class VideoWidget;

class Renderer : public QObject
{
    Q_OBJECT

public:
    Renderer(VideoWidget *video);
    ~Renderer();

    int render(uint32_t type, void *args[]);
    int renderBackground();
    int renderRect(uint16_t width, uint16_t height, const RectA &rect);
    void emitFlushImage();

    void setMode(uint32_t mode)
    {
        m_mode = mode;
    }

    Frame8 m_rawFrame;
    ProcessBlobs m_blobs;


signals:
    void image(QImage image);
    void flushImage();

private:
    inline void interpolateBayer(unsigned int width, unsigned int x, unsigned int y, unsigned char *pixel, unsigned int &r, unsigned int &g, unsigned int &b);

    int renderCCQ1(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t numVals, uint32_t *qVals);
    int renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
    int renderCCB1(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t numBlobs, uint16_t *blobs);
    int renderCMV1(uint8_t renderFlags, uint32_t cmodelsLen, float *cmodels, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);
    void emitImage(const QImage &image);

    int renderBA81Filter(uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);

    void handleRL(QImage *image, uint color, uint row, uint startCol, uint len);

    VideoWidget *m_video;
    bool m_backgroundFrame; // our own copy because we're in a different thread (not gui thread)
    QImage m_background;

    uint32_t m_mode;
};

#endif // RENDERER_H

