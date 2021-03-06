#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <memory>

#include "cluster.h"
using namespace std;

#ifdef _E_
#define Err 1
#else
#define Err 0
#endif

namespace Cluster{

typedef shared_ptr<site> sitePtr;

static int clusterIdCounter = 0;
static int thresh = 0;

int setThresh(int n){
    if(n>0){
        thresh=n;
        if(Err) cout<<"Thresh hold is: "<<thresh<<endl;
        return 1;
    }else{
        if(Err) cerr<<"ERROR in setThresh"<<endl;
        return 0;
    }
}

int getThresh(){
	return thresh;
}
/*
cluster::cluster(int            siteId1,  //simpler constructer and use an add function
		 vector<int>    neighId1,
		 vector<double> neighRates1,
		 int            sizen1,
		 int            visitFreq1,
		 int            siteId2,
		 vector<int>    neighId2,
		 vector<double> neighRates2,
		 int            sizen2,
		 int            visitFreq2){
*/		 
cluster::cluster(){
	clusterId=clusterIdCounter;
	clusterIdCounter++;
	//site  * tmp = new site(siteId1,neighRates1,neighId1,sizen1,visitFreq1);
	//site * tmp2 = new site(siteId2,neighRates2,neighId2,sizen2,visitFreq2);
//	shared_ptr<site> tmp = site(siteId1,vector<double> neighRates,vector<int> neighId1,visitFreq1);
	//create cluster then add
//	sitesInCluster.push_back(tmp);
//	sitesInCluster.push_back(tmp2);	
}

cluster::~cluster(){
}

int cluster::addSite(sitePtr newSite){ //call potential cluster in add
	/* add site to vector
	 * assign cluster Id to site
	 * remove site from neighbor list
	 * add unqiue neighbors to list 
	 */

	if(newSite == NULL){
		if(Err) cerr<<"Adding Null site"<<endl;
		return 0;
	}

	newSite->clustTag = clusterId;

	sitesInCluster.push_back(newSite);
	
	int addFlag;
	for(int i = 0; i < (int) newSite->neighIds.size(); i++){
		addFlag = 1;
		for(int j = 0; j < (int) neighIdsCluster.size(); j++){
			if(neighIdsCluster[j] == newSite->siteId){
				neighIdsCluster.erase(neighIdsCluster.begin() + j);
			}
			if(newSite->neighIds[i] == neighIdsCluster[j]){
				addFlag = 0;
			}
		}
		if(addFlag){
			neighIdsCluster.push_back(newSite->neighIds[i]);
		}
	}
	return 1;
}

int cluster::getClusterId(){
	return clusterId;
}


vector<sitePtr> cluster::getSitesInCluster(){
	return sitesInCluster;
}


int cluster::printClusterInfo(){
	cout<<"Cluster ID: "<<clusterId<<endl;
	cout<<"Visit Frequency to Cluster: "<< visitFreqCluster<<endl;
	cout<<"Sites in Cluster: "<<endl;
	for(int i = 0; i < (int)sitesInCluster.size(); i++){
		cout<<"Site Id: "<<sitesInCluster[i]->siteId<<" Visit Frequency: "<<sitesInCluster[i]->visitFreq<<endl;
	}
	return 1;
}

/*
int potentialCluster(int visitFreq1, int visitFreq2){
	if(visitFreq1<0||visitFreq2<0||thresh<0){
		fprintf(stderr,"ERROR in potentialCluster\n");//find a way to turn off in preprocessor
		return -1;
	}
	if((visitFreq1 > thresh) && (visitFreq2 > thresh))
		return 1;
	return 0;
}

int clusterOrSite(int clusterId1, int clusterId2){
	if(clusterId1<-1||clusterId2<-1)
		return -1;
	if(clusterId1 == -1 && clusterId2 == -1)
		return 1;
	if(clusterId1 == -1 || clusterId2 == -1)
		return 2;
	return 3;
}
*/

/*
int cluster::neighSiteCluster(site * site1, site * site2, int * neighCluster){
	int i=0, j=0;
	if(site1 ==NULL ||site2==NULL||neighCluster==NULL){
		if(Err) cerr<<"ERROR in neighSiteCluster"<<endl;
		return -1;
	}
	while(site1->neighIds[i]!=-1){ //Use -1 as a terminator
		neighCluster[i]=site1->neighIds[i];
		i++;
	}
	while(site2->neighIds[j]!=-1){
		neighCluster[i+j]=site2->neighIds[j];
		j++;
	}
	return 1;
}
*/


double cluster::dwellTime(){
	sitePtr tmp;
	double sum;
	for(int i =0; i < (int)sitesInCluster.size(); i++){
		tmp=sitesInCluster[i];
		for(int j =0; j < (int)tmp->neighRates.size(); j++){
			sum += tmp->neighRates[j];
		}
	}
	cout<<"Sum of Cluster rates:"<<sum<<endl;
	return (1/sum);
}


double cluster::probHop(sitePtr shipping, sitePtr receiving){ 
	if(shipping->clustTag != receiving ->clustTag){
		if(Err) cerr<<"IN probHop: shipping and recieving dont belong to the same cluster."<<endl;
		return -1;
	}

	int errFlag = 0;
	for(int i = 0; i< (int)sitesInCluster.size(); i++){ //check
		int tmp = sitesInCluster[i]->siteId;
		if(tmp == receiving->siteId){
		       	errFlag = 1;
		}
	}
	if(errFlag==0){
		if(Err)	cerr<<"Target not conencted to cluster"<<endl;
		return -1;
	}

	int index = -1;
	for(int i = 0; i<(int)shipping->neighIds.size(); i++){ 
	//Improvement: combine all for loops
		if(receiving->siteId==shipping->neighIds[i]) index = i;
	}
	int totalRates =0;
	for(int i =0; i<(int)shipping->neighRates.size(); i++){
		totalRates+=shipping->neighRates[i];
	}
	return shipping->neighRates[index]/totalRates;
}

/*
double cluster::hopOffCluster(site * target){
	site * tmp;
	int totalRates=0;
	for(int i =0;i++;i<SitesInCluster.size()){ //figure out pops, pushbacks, and .size() thing
		tmp=SitesInCluster.pop(); 
		for(int j =0; j++; j< sizen){
			totalRates+=nRates[i]; //could calculate at instantation?
		}
	}
	//find way to get target to cluster rate
	return targetToCluster/totalRates
}

double cluster::escapeTime(site * jail){
*/

int cluster::intializeProbOnSite(){
	for(int i =0; i < (int) sitesInCluster.size(); i++){
		sitesInCluster[i]->ProbOnSite = 1/( (int) sitesInCluster.size());
	}
	if(Err) cout<<"Intializing Probs On site"<<endl;
	return 1;
}

//int cluster::clusterConvergence(long interations){
	/*intialize probabilities
	 * for every interation..,
	 * for every site in the cluster
	 * find all neighboring sites of that site that are also in the cluster
	 * use prob hop function to get new prod on site
	 * normalize new prob on site -> new prob on site / sum of all new prob on site
	 * average new prob on site with old
	 */
/*
	assert(itializeProbOnSite());
	for(long i = 0; i < interations){
		for(int j = 0; j < sitesInCluster.size(); i++>){
		}
	}

			
}*/
}
