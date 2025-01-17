
#ifndef STATISTICINFO_H
#define STATISTICINFO_H


class StatisticInfo {
private:
    int rightDecision;
    int wrongDecision;
    int total;
    int percentage;
public:
    StatisticInfo();
    void increaseRightDecisions(int i);
    void increaseRightDecision();
    void increaseWrongDecision();
    void increaseWrongDecisions(int i);
    void increaseTotal();
    void increaseTotals(int i);
    int getRightDecision();
    int getWrongDecision();
    int getTotal();
    int getPercentage();
    void calculatePercentage();
    void printStatistics();
    void reset();

};


#endif
