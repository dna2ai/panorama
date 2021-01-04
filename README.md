# panorama
use ezSIFT to glue photos together

port ezSIFT to javascript with emscripten

`build_ezsift_with_em.sh` is used to generate `ezsift.js`

`ezsift.cpp` is main source file for `ezsift.js` to export APIs from ezSIFT
(much code copied from `feature_extract.cpp` and `image_match.cpp`)

Currently we support APIs:
- `Module._createImage(w, h) -> img` to create a 8bit image
- `Module._disposeImage(img)` to release memory alloc
- `Module._putPixel(img, x, y, val)`, where 0 <= val < 256
- `Module._getPixel(img, x, y) -> val`
- `Module._extractFeature(img)` to extract feature into an internal list `rKeypoints`
- `Module._backupKeypoints(index)`, where 0 <= index < 10, to save current internal `rKeypoints` into a cell at index.
- `Module._restoreKeypoints(index)`, to load the saved list assigning to internal `rKeypoints`
- `Module._getKeypointsN()` to get current the number of key points in `rKeypoints`
- `Module._keypointBegin()` to assign the first key point to cursor in `rKeypoints`
- `Module._keypointNext()` to get next key point in `rKeypoints`
- `Module._keypointHasNext()` to check if has next key point
- `Module._keypointOctave()` `_keypointLayer()` `_keypointR()` `_keypointC()` `_keypointScale()` `_keypointOri()` `_keypointDescriptor(index), where 0 <= index < 128` to get different key point attributes
- `Module._keypointsMatch(indexA, indexB)`, where 0 <= indexA, indexB < 10, to match to saved key point list in 2 cells; save to internal list `rMatch`
- `Module._getMatchN()` to get the number of matched key points in `rMatch`
- `Module._matchBegin()` to assign the first matched key point to cursor in `rMatch`
- `Module._matchNext()` to get next matched key point in `rMatch`
- `Module._matchHasNext()` to check if has next matched key point
- `Module._matchR1()` `_matchC1()` `_matchR2()` `_matchC2()` to get matched key point position `(c1, r1) -> (c2, r2)`; in ezSIFT, `c` represents `x` meanwhile `r` represents `y` in real image

after get `ezsift.js`,
- put into `dist` folder;
- create a folder `local` and put `1.png` `2.png` into the folder;
- run `python -m SimpleHTTPServer` or `python3 -m http.server`
- open `http://127.0.0.1:8000/test.html` (`canvasContext.getImageData()` do not work when directly open `test.html` in file protocol due to cross-origin)

wait for seconds, you will see a canvas drawing matched points between 2 images.
