# weekend-ray-tracer
Ray tracer based the book "[Ray Tracing in One Weekend](https://www.amazon.com/Ray-Tracing-Weekend-Minibooks-Book-ebook/dp/B01B5AODD8)"

To compile on OS X El Capitan (10.11.6)
```
g++ -o rt rt.cpp -L/usr/include/c++/4.2.1/
```

To run
```
./rt > img.ppm
```

To view output you will need something to view images in PPM format. The book recommends
[ToyViewer](http://www7a.biglobe.ne.jp/~ogihara/en/Mac_OS_X.html). You can also get this
from the AppStore.
