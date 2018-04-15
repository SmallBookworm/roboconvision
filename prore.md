---
markdown:
  image_dir: /assets
  path: README.md
  ignore_from_front_matter: true
  absolute_image_path: false
export_on_save:
    markdown: true
---
# Required Packages
* GCC 5.4.x or later
* Cmake 3.5 or higher
* [OpenCV 3.0.0 or higher](https://docs.opencv.org/3.4.1/d7/d9f/tutorial_linux_install.html)
* [Intel® RealSense™ SDK 2.0](https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md)

# Program Construct(undone)
```viz{filename="ad.png"}
digraph G{
    node[shape="record"];
    subThread1[label="drok"];
    subThread2[label="drop"];
    elec[label="lower system"]
    subgraph cluster_communiction{
       lable="mainThread"
       bgcolor="beige"
       info1[label="LineInfo"]
       info2[label="DeviationPosition"]
       main[label="main"]
       serial
    }
    subThread1->info1[label="info" dir=both];
    subThread2->info2[label="info" dir=both];
    info1,info2->main[label="info" dir=both];
    main->subThread1[label="detach" style=dotted];
    main->subThread2[label="detach" style=dotted];
    main->serial;
    serial->elec;
}
```