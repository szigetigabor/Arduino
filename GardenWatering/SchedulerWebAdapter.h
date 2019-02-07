//  SchedulerWebAdapter.h - Arduino Scheduler web adapter class
#pragma once
#ifndef SCHEDULER_WEB_ADAPTER_H
#define SCHEDULER_WEB_ADAPTER_H

class SchedulerWebAdapter
{
  public:
    SchedulerWebAdapter(SchedulerLogic* schedulerLogic);

    String getStartTime();
    void setStartTime(int hour, int minute);
    
  private:
    SchedulerLogic* mschedulerLogic;
};

//**************************************************************
//* SchedulerWebAdapter Class Constructor

SchedulerWebAdapter::SchedulerWebAdapter(SchedulerLogic* schedulerLogic){
  mschedulerLogic = schedulerLogic;
}

//**************************************************************
//* Public Methods

String SchedulerWebAdapter::getStartTime() {
  String retval = String(mschedulerLogic->getStartHour());
  retval += ":";
  retval += mschedulerLogic->getStartMinute();
  return retval;
}


void SchedulerWebAdapter::setStartTime(int hour, int minute) {
  mschedulerLogic->setStartHour(hour);
  mschedulerLogic->setStartMinute(minute);
}


//**************************************************************
//* Private Methods

#endif  //SCHEDULER_WEB_ADAPTER_H
