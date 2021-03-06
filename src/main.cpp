/*
Author:         Jianming Zhang
Note:           This application computes CLEAR MOT metric for 2D tracking.
                Objects swapping IDs will be counted as 2 ID switches. If you
                specify a sequence path for 'calMOT()', the matching results will
                be displayed frame by frame. 
                                        
                Green thin and thick boxes: good matches;
                Red thin boxes: missed targets; 
                Red thick boxes: false alarms;
                Yellow thin and thick boxes: ID swithes; 
*/

#include <iostream>
#include <sstream>
#include <fstream>
#include <time.h>

#include "libxml/parser.h"
#include "libxml/tree.h"
#include "libxml/xmlmemory.h"

#include "clearMOT.h"
#include "dataReader.h"

#define GT_XML_FILE "../groundtruth/test_set_truth.xml"

#define HP_XML_FILE "../results/network_DenseNet121_version_0_dataset_0_confidency_0.6_test_set.xml"

static bool isGroundtruth = false;

class SeqReader
{
	int frameCount;
	string path;

public:
	SeqReader(const string p):frameCount(0),path(p){}
	void readImg(Mat& frame)
	{
		string fileName=path+"frame_";
		char _count[10];
		sprintf(_count,"%d",frameCount);
		string count=_count;
		count.insert(count.begin(),4-count.length(),'0');
		count=count+".jpg";
		fileName=fileName+count;
		frame=imread(fileName);
		frameCount++;
	}
};

int calMOT(const char* path=NULL)
{
	string gtName, hpName;
	
	gtName = GT_XML_FILE;
	
	if (isGroundtruth) hpName = gtName;
	else hpName = HP_XML_FILE;
	
	ResultParser gt(gtName.c_str(),1.0);
	ResultParser hp(hpName.c_str(),    1.0,                   	1.0,                    1.0);//you may scale the result bounding box: w=w*r*w_r, h=h*r*h_r
	//                                 [ratio]					[width_ratio]			[height_ratio]
	C_Mot mot(1.0);//1.0: IOU threshold     

	if (path==NULL)
	{
		while(!gt.isEnd() && !hp.isEnd())
			mot.dealWithDetection(gt.readNextFrame(),hp.readNextFrame());

		mot.getMOT(true);
		return 0;
	}

	SeqReader reader=SeqReader(string(path));
	Mat frame;
	//namedWindow("multiTrack",CV_WINDOW_AUTOSIZE);

	reader.readImg(frame);
	if (frame.data==NULL)
	{
		cerr<<"fail to read sequence!"<<endl;
		return 0;
	}
	mot.dealWithDetection(gt.readNextFrame(),hp.readNextFrame());
	//mot.paintFrame(frame,isGroundtruth);
	//imshow("multiTrack",frame);
	//waitKey(0);
	while(!gt.isEnd() && !hp.isEnd())
	{
		reader.readImg(frame);
		mot.dealWithDetection(gt.readNextFrame(),hp.readNextFrame());
		//mot.paintFrame(frame,isGroundtruth);
		//imshow("multiTrack",frame);
		//waitKey(0);
	}
	mot.getMOT(true);
	
	return 0;
}

int main(int argc, char** argv)
{
	char* path = 0;
	
	if ((argc < 2) || (argc >= 4))
	{
		cerr << "Wrong syntax. Usage: ./ClearMOT <is-groundtruth> [ <dataset-path> ]." << endl;
		
		exit(-1);
	}
	
	if (strcmp(argv[1],"0") == 0) isGroundtruth = false;
	else if (strcmp(argv[1],"1") == 0) isGroundtruth = true;
	else
	{
		cerr << "Wrong syntax. Usage: ./ClearMOT < 1 | 0 > [ <dataset-path> ]." << endl;
		
		exit(-1);
	}
	
	if (argc == 3) path = argv[2];
	
	calMOT(path);
	
	return 0;
}
