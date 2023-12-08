### DB-Grp-D4

This is a brief guide on how to run the C++ implementation of the "Data Driven Domain Discovery for Structured Datasets" project. This project aims to find robust signatures for different domains in a structured dataset i.e NYC education Dataset 

The first step is to download all the required dataset files which can be found in this repository other datafiles can be downloaded from below link. 
https://zenodo.org/records/3647642

The second step is to download the Robust Signatures.txt file, which contains the list of robust signatures for each domain. These signatures are sets of words or phrases that are highly indicative of a specific domain and can be used to classify new records.

The third step is to modify the two algorithms that are provided in the project folder: DomainDiscovery.cpp and DomainClassification.cpp. These algorithms implement the main steps of the project: finding robust signatures for each domain and using them to classify new records. In both algorithms, you need to give the right path for loading the dataset files that you downloaded in the first step. For 


The fourth and final step is to compile and run the algorithms using a C++ compiler. You can use any compiler that supports C++11 or higher, such as GCC, Clang, or Visual Studio. To compile and run the algorithms using GCC, you need to type the following commands in a terminal:

g++ DomainDiscovery.cpp -o DomainDiscovery
./DomainDiscovery

g++ DomainClassification.cpp -o DomainClassification
./DomainClassification

These commands will generate two outputs.
