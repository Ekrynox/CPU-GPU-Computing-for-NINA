# NINA++

## 0.5.3.1
- Add the instruction to install VC Redist X64 in Description inside NINA

## 0.5.3.0
- Optimize data transfer to GPU for Debayering & ResizeBicubic
- Optimize GPU Algorithm for ResizeBicubic & Convolution
- Optimize CPU & GPU Algorithm for Debayering

## 0.5.2.0
- Update description and add small guidelines to the setting UI
- Implement OpenCL Accord: Filters: Convolution
- Fix on OpenCL Accord: Filters: Convolution, when startX/stopX are not 0
- Add a 5-second timer before applying new settings (Harmony Patch & OpenCL compilation)

## 0.5.1.0
- Implement OpenCL Accord: Filters: ResizeBicubic
- Rename project

## 0.5.0.0
- Implement OpenCL debayering
- Optimize loop management for multithreaded functions
- Fix Settings error on 3.2RC7

## 0.4.0.0
- Add Settings to enable/disable functions patch
- Add Settings to toggle Multithreaded variants
- Add Multithreaded
    - Accord: Filters: Convolution
    - Accord: Filters: ResizeBicubic
	- NINA: Image: ImageAnalysis: BayerFilter16bpp

## 0.3.2.0
- Auto Manifest generation

## 0.3.1.0
- Reduce branching inside BayerFilter16bpp loops

## 0.3.0.0
- Add Accord: Filters: BinaryDilation3x3
- Add Accord: Filters: CannyEdgeDetector
- Add Accord: Filters: Convolution
- Add Accord: Filters: NoBlurCannyEdgeDetector
- Add Accord: Filters: ResizeBicubic
- Add Accord: Filters: SISThreshold

## 0.2.0.0
- Add FastGaussianBlur
- Add ColorRemappingGeneral
- Add Accord: StarDetection::Star::Calculate
- Add Accord: BlobCounter
- Add Accord: BlobCounterBase::CollectObjectsInfo

## 0.1.0.0
- Add Debayering to C++
