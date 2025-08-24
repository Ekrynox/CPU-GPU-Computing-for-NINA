#include "BlobCounter.hpp"

#include "../../Image/enums.hpp"



namespace LucasAlias::NINA::NinaPP::Accord::Imaging {

    void mapObject8bpp(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t stride, int32_t* ObjectLabels, int32_t& labelsCount, int32_t* map, uint8_t backgroundThresholdG) {
        int32_t p = 0;
        int32_t offset = stride - ImageWidth;
        int32_t imageWidthM1 = ImageWidth - 1;

        // 1 - for pixels of the first row
        if (*src > backgroundThresholdG) ObjectLabels[p] = ++labelsCount;
        ++src;
        ++p;

        // process the rest of the first row
        for (int32_t x = 1; x < ImageWidth; x++, src++, p++) {
            // check if we need to label current pixel
            if (*src > backgroundThresholdG) {

                // check if the previous pixel already was labeled
                if (src[-1] > backgroundThresholdG) {
                    // label current pixel, as the previous
                    ObjectLabels[p] = ObjectLabels[p - 1];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
        }
        src += offset;

        // 2 - for other rows
        // for each row
        for (int32_t y = 1; y < ImageHeight; y++) {
            // for the first pixel of the row, we need to check
            // only upper and upper-right pixels

            if (*src > backgroundThresholdG) {
                // check surrounding pixels
                if (src[-stride] > backgroundThresholdG) {
                    // label current pixel, as the above
                    ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                }
                else if (src[1 - stride] > backgroundThresholdG) {
                    // label current pixel, as the above right
                    ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
            ++src;
            ++p;

            // check left pixel and three upper pixels for the rest of pixels
            for (int32_t x = 1; x < imageWidthM1; x++, src++, p++) {
                if (*src > backgroundThresholdG) {
                    // check surrounding pixels
                    if (src[-1] > backgroundThresholdG) {
                        // label current pixel, as the left
                        ObjectLabels[p] = ObjectLabels[p - 1];
                    }
                    else if (src[-1 - stride] > backgroundThresholdG) {
                        // label current pixel, as the above left
                        ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                    }
                    else if (src[-stride] > backgroundThresholdG) {
                        // label current pixel, as the above
                        ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                    }

                    if (src[1 - stride] > backgroundThresholdG) {
                        if (ObjectLabels[p] == 0) {
                            // label current pixel, as the above right
                            ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                        }
                        else {
                            int32_t l1 = ObjectLabels[p];
                            int32_t l2 = ObjectLabels[p + 1 - ImageWidth];

                            if ((l1 != l2) && (map[l1] != map[l2])) {
                                // merge
                                if (map[l1] == l1) {
                                    // map left value to the right
                                    map[l1] = map[l2];
                                }
                                else if (map[l2] == l2) {
                                    // map right value to the left
                                    map[l2] = map[l1];
                                }
                                else {
                                    // both values already mapped
                                    map[map[l1]] = map[l2];
                                    map[l1] = map[l2];
                                }

                                // reindex
                                for (int32_t i = 1; i <= labelsCount; i++) {
                                    if (map[i] != i) {
                                        // reindex
                                        int32_t j = map[i];
                                        while (j != map[j])
                                        {
                                            j = map[j];
                                        }
                                        map[i] = j;
                                    }
                                }
                            }
                        }
                    }

                    // label the object if it is not yet
                    if (ObjectLabels[p] == 0) {
                        // create new label
                        ObjectLabels[p] = ++labelsCount;
                    }
                }
            }

            // for the last pixel of the row, we need to check
            // only upper and upper-left pixels
            if (*src > backgroundThresholdG)
            {
                // check surrounding pixels
                if (src[-1] > backgroundThresholdG) {
                    // label current pixel, as the left
                    ObjectLabels[p] = ObjectLabels[p - 1];
                }
                else if (src[-1 - stride] > backgroundThresholdG) {
                    // label current pixel, as the above left
                    ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                }
                else if (src[-stride] > backgroundThresholdG) {
                    // label current pixel, as the above
                    ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
            ++src;
            ++p;

            src += offset;
        }
    }

    void mapObjectRGB(uint8_t* src, const int32_t ImageWidth, const int32_t ImageHeight, const int32_t pixelSize, const int32_t stride, int32_t* ObjectLabels, int32_t& labelsCount, int32_t* map, uint8_t backgroundThresholdR, uint8_t backgroundThresholdG, uint8_t backgroundThresholdB) {
        int32_t p = 0;

        // color images
        int32_t offset = stride - ImageWidth * pixelSize;

        int32_t strideM1 = stride - pixelSize;
        int32_t strideP1 = stride + pixelSize;

        // 1 - for pixels of the first row
        if ((src[RGB::R] | src[::RGB::G] | src[::RGB::B]) != 0) ObjectLabels[p] = ++labelsCount;
        src += pixelSize;
        ++p;

        // process the rest of the first row
        for (int32_t x = 1; x < ImageWidth; x++, src += pixelSize, p++) {
            // check if we need to label current pixel
            if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                // check if the previous pixel already was labeled
                if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                    // label current pixel, as the previous
                    ObjectLabels[p] = ObjectLabels[p - 1];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
        }
        src += offset;

        // 2 - for other rows
        // for each row
        for (int32_t y = 1; y < ImageHeight; y++) {
            // for the first pixel of the row, we need to check
            // only upper and upper-right pixels
            if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                // check surrounding pixels
                if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                    // label current pixel, as the above
                    ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                }
                else if ((src[::RGB::R - strideM1] > backgroundThresholdR) || (src[::RGB::G - strideM1] > backgroundThresholdG) || (src[::RGB::B - strideM1] > backgroundThresholdB)) {
                    // label current pixel, as the above right
                    ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
            src += pixelSize;
            ++p;

            // check left pixel and three upper pixels for the rest of pixels
            for (int32_t x = 1; x < ImageWidth - 1; x++, src += pixelSize, p++) {
                if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                    // check surrounding pixels
                    if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                        // label current pixel, as the left
                        ObjectLabels[p] = ObjectLabels[p - 1];
                    }
                    else if ((src[::RGB::R - strideP1] > backgroundThresholdR) || (src[::RGB::G - strideP1] > backgroundThresholdG) || (src[::RGB::B - strideP1] > backgroundThresholdB)) {
                        // label current pixel, as the above left
                        ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                    }
                    else if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                        // label current pixel, as the above
                        ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                    }

                    if ((src[::RGB::R - strideM1] > backgroundThresholdR) || (src[::RGB::G - strideM1] > backgroundThresholdG) || (src[::RGB::B - strideM1] > backgroundThresholdB)) {
                        if (ObjectLabels[p] == 0) {
                            // label current pixel, as the above right
                            ObjectLabels[p] = ObjectLabels[p + 1 - ImageWidth];
                        }
                        else {
                            int32_t l1 = ObjectLabels[p];
                            int32_t l2 = ObjectLabels[p + 1 - ImageWidth];

                            if ((l1 != l2) && (map[l1] != map[l2])) {
                                // merge
                                if (map[l1] == l1) {
                                    // map left value to the right
                                    map[l1] = map[l2];
                                }
                                else if (map[l2] == l2) {
                                    // map right value to the left
                                    map[l2] = map[l1];
                                }
                                else {
                                    // both values already mapped
                                    map[map[l1]] = map[l2];
                                    map[l1] = map[l2];
                                }

                                // reindex
                                for (int32_t i = 1; i <= labelsCount; i++) {
                                    if (map[i] != i) {
                                        // reindex
                                        int32_t j = map[i];
                                        while (j != map[j]) {
                                            j = map[j];
                                        }
                                        map[i] = j;
                                    }
                                }
                            }
                        }
                    }

                    // label the object if it is not yet
                    if (ObjectLabels[p] == 0) {
                        // create new label
                        ObjectLabels[p] = ++labelsCount;
                    }
                }
            }

            // for the last pixel of the row, we need to check
            // only upper and upper-left pixels
            if ((src[::RGB::R] > backgroundThresholdR) || (src[::RGB::G] > backgroundThresholdG) || (src[::RGB::B] > backgroundThresholdB)) {
                // check surrounding pixels
                if ((src[::RGB::R - pixelSize] > backgroundThresholdR) || (src[::RGB::G - pixelSize] > backgroundThresholdG) || (src[::RGB::B - pixelSize] > backgroundThresholdB)) {
                    // label current pixel, as the left
                    ObjectLabels[p] = ObjectLabels[p - 1];
                }
                else if ((src[::RGB::R - strideP1] > backgroundThresholdR) || (src[::RGB::G - strideP1] > backgroundThresholdG) || (src[::RGB::B - strideP1] > backgroundThresholdB)) {
                    // label current pixel, as the above left
                    ObjectLabels[p] = ObjectLabels[p - 1 - ImageWidth];
                }
                else if ((src[::RGB::R - stride] > backgroundThresholdR) || (src[::RGB::G - stride] > backgroundThresholdG) || (src[::RGB::B - stride] > backgroundThresholdB)) {
                    // label current pixel, as the above
                    ObjectLabels[p] = ObjectLabels[p - ImageWidth];
                }
                else {
                    // create new label
                    ObjectLabels[p] = ++labelsCount;
                }
            }
            src += pixelSize;
            ++p;

            src += offset;
        }
    }

    void reMapObject(int32_t* ObjectLabels, const int32_t ObjectLabelsLen, const int32_t labelsCount, int32_t* map, int32_t& ObjectsCount, int32_t* reMap) {
        // count objects and prepare remapping array
        ObjectsCount = 0;
        for (int i = 1; i <= labelsCount; i++) {
            if (map[i] == i) {
                // increase objects count
                reMap[i] = ++ObjectsCount;
            }
        }
        // second pass to complete remapping
        for (int i = 1; i <= labelsCount; i++) {
            if (map[i] != i) reMap[i] = reMap[map[i]];
        }
        // repair object labels
        for (int i = 0, n = ObjectLabelsLen; i < n; i++) {
            ObjectLabels[i] = reMap[ObjectLabels[i]];
        }
    }
}