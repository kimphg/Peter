#include <QCoreApplication>
#include "c_decision_tree.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    c_decision_tree myTree;
    std::vector<QString> features;
    features.push_back("rgAziErr");
    features.push_back("dtime");
    features.push_back("speedkmh");
    features.push_back("distancekm");
    myTree.LoadDataFile("logfile.dat",features);
    myTree.run();
    DataPoint point = myTree.mDataSet.at(myTree.mDataSet.size()/2);
    double predict = myTree.Predict(point);
    return a.exec();
}
