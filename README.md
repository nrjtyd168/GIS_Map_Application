# GIS_Map_Application
GIS based map application built in C++ language using Qt Framework in Linux environment and ArcGIS Run Time SDK for Qt. The application software is developed to receive platform data and display its parameters in different modes (moving map, moving platform and pan mode) on a map window of a menu-based Qt application. 
The application provides features and supportive tools that are useful for – Adding Layers, Annotations, Waypoints, Switching Map Modes, Scaling & Zooming, Depicting Platform Path and Finding Distance & Bearing. 

This GIS based map application software is developed under Red Hat Enterprise Linux (RHEL) 8.5 operating system with Qt 5.15.2 as the development framework using C++ language. Qt Creator 7.0.2 is used as the Integrated Development Environment (IDE).  Arc GIS Run Time SDK 100.13.1 is used as the GIS framework for implementing map functionalities. 

Multicast based Qt application is developed for simulating and sending platform data to this map application through Ethernet LAN using UDP/IP protocol. The simulation software can simulate data for multiple platforms.

The platform data received from the simulator is displayed on map window of the map application software. Appropriate symbols are assigned to each platform to differentiate them separately. A user-friendly GUI is designed and developed to view the received platform data on the application window. Geo-Tiff based maps are used as underlay base maps.

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/a679dc62-0ca5-4198-bb14-9d93600c56f9)

## Overview of Geographic Information System
A Geographic Information System (GIS) is a system that creates, manages, analyses, and maps all types of data. GIS connects data to a map, integrating location data (where things are) with all types of descriptive information (what things are like there). This provides a foundation for mapping and analysis that is used in science and almost every industry. GIS helps users understand patterns, relationships, and geographic context. The benefits include improved communication and efficiency as well as better management and decision-making.

A geographic information system (GIS) is a computer system for capturing, storing, checking, and displaying data related to positions on Earth’s surface. By relating seemingly unrelated data, GIS can help individuals and organizations for better understanding of spatial patterns and relationships.
GIS can use any information that includes location. The location can be expressed in many different ways, such as latitude and longitude, address, or ZIP code.

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/77002af4-96a3-4b07-a012-3938e6f17fbc)

## Features and Tools
### Loading Map Layer
The primary role of a Map Layer is to help people visualize the interplay between the physical characteristics of a particular geographical area and the proposed development, or project. While every base map is unique, most maps contain layers that include one or more of the following common GIS data or imagery: Streets, roads, and highways, Bodies of water such as rivers, lakes, and streams, Geographical boundaries (city, county, state or personal property boundaries) Satellite imagery.

### Annotation
Annotation in the geo-database is stored in annotation feature classes. As with other feature classes, all features in an annotation feature class have a geographic location and attributes and can either be inside a feature dataset or a standalone feature class. Each annotation feature has symbols including font, size, color, and any other text symbol property. 

### Waypoint
Waypoints are very much similar to annotations but are a chain of points connected to each other indicated by lines. Waypoints also has symbols and other type of graphic shapes. Waypoints can also be physical things that hold navigation devices: ships in the ocean or satellites in the sky, for example. These types of waypoints are used for collecting data.

### Distance and Bearing
The Distance and Bearing Tool can be used to describe the direction and distance between individual point features, along a line feature, or around a polygon feature. The tool examines the currently selected feature the features were digitized (points), or the order (lines and polygons). Bearing is expressed as an angle of rotation in degrees-minutes seconds west (or east) of the north (or south). Distance is always expressed in miles, regardless of map units. If more than two point features are selected, the tool assumes that the points describe a polygonal area and will “close the loop” between the last point and the first point. Bearing and distance figures are added as graphic text in the “ArcMap annotation group.

 ### Platform Path
Platforms refer to the structures or vehicles on which remote sensing instruments are mounted. The platform on which a particular sensor is housed determines the number of attributes, which may dictate the use of particular sensors. These attributes include the distance the sensor is from the object of interest, periodicity of image acquisition, the timing of image acquisition, and location and extent of coverage. There are four broad categories of remote sensing platforms: ground-based, waterborne, airborne, and satellite.

### Map Modes
        Moving Map
This tool keeps the platform at the center of the layout window and moves the map, synchronizing with the moving platform. It helps in avoiding the consequence of losing the view of the platform from the map scale to the layout window.

        Moving Platform
This tool recenters the platform when it moves out of the map view frame scaled to the layout window. It helps in avoiding the consequence of losing the view of the platform from the map scale to the layout window.

        Pan Mode
This tool allows us to grab the map and move to change the map view by providing a hand cursor. The hand cursor is open when it is not grabbing and closed when it grabs the map.

## Frameworks and SDKs

### Qt Framework
Qt is a cross-platform application development framework for desktop, embedded, and mobile. Supported Platforms include Linux, OS X, Windows, VxWorks, QNX, Android, iOS, BlackBerry, Sailfish OS, and others.

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/5d82ca78-6fbd-46d7-baf1-51ce8d339e3b)

### ArcGIS Runtime SDK
ArcGIS Runtime APIs are APIs that are used to build mapping, location, and GIS applications for phones, laptops, desktops, and other devices. These applications can view and edit maps and perform the analysis while offline, and can directly access a device's storage, sensors, GPU, and other capabilities. ArcGIS Runtime APIs are available for Android, iOS, Java, .NET, and Qt to build applications that run on WPF, UWP, iOS, Android, Windows, Linux, and macOS.

## System Architecture
![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/4d94281f-ce8a-4225-aac8-ffb57deeb0c5)

## Hardware Configuration
•	Processor	:	Intel based Processor (Core i5)

•	Speed		:	1 GHz

•	RAM		:	2 GB

•	Hard Disk	:	200 GB

•	Keyboard	:	Standard Keyboard

•	Monitor	        :	SVGA or HDMI

## Software Configuration
•	Operating System		:	Red Hat Enterprise Linux 8.5 or later

•	Programming Language	        :	C++ 14

•	Framework			:	Qt 5.15.2

•	Tool				:	Qt Creator 7.0.2

•	SDK				:	ArcGIS Runtime SDK 100.13.1

•	Database			:	MySQL

## Screenshots
![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/734a5bc3-2bb0-458b-b03c-18f99ce16db3)

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/b5ea7998-2e78-493a-9e4c-5db6b347879e)

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/669afe62-d4af-445b-b2d4-86ea1cc99d09)

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/dd077eca-8d86-48b6-927e-db0e997cb890)

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/cc2e18a1-f06d-4c9c-9d75-58462a2451b7)

![image](https://github.com/nrjtyd168/GIS_Map_Application/assets/89921374/0f6711f2-b6ea-43bb-9eb1-d31d326800e2)










