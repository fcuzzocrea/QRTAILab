#include "data2disk.h"



QRL_Data2Disk::QRL_Data2Disk(float d)
      :  dt(d)
    {

    Is_Target_Saving=0;
     saveFormat=ascii;
     seconds=0;
     save_file=NULL;
     saveScopeTime=false;
     fileName=tr("test");
}

QRL_Data2Disk::~QRL_Data2Disk(){


}

bool QRL_Data2Disk::startSaving(const char * name, double save_time){
    bool ret;
    ret=openSaveFile(name);
    if (ret){
        saveTime=save_time;
        return true;
    } else
        return false;
}

 bool QRL_Data2Disk::openSaveFile(const char * name){

 if (saveFormat==ascii){
  if ((save_file = fopen(name, "w+")) == NULL) {
        Is_Target_Saving=0;
      return false;

  } else {
    Is_Target_Saving=1;
    fileName=QString(name);
    return true;
  }
} else
    return false;

 }

  void QRL_Data2Disk::closeSaveFile(){

    fclose(save_file);
    save_file=NULL;
  }

   void QRL_Data2Disk::stopSaving(){
      closeSaveFile();
      Is_Target_Saving=0;
   }

   void QRL_Data2Disk::writeNextData(double d,int a){
    switch(a){
       case 5:
        fprintf(save_file, "%1.5f ", d);
        break;
        case 10:
        default:
         fprintf(save_file, "%1.10f ", d);

    }

   }
    void QRL_Data2Disk::newLine(){

        fprintf(save_file, "\n");

    }


      int  QRL_Data2Disk::n_points_to_save(){
        int n_points;

        n_points = (int)(saveTime/dt);
        if (n_points < 0) return 0;
        return n_points;

}

  void  QRL_Data2Disk::set_points_counter(int cnt){

  Saved_Points=cnt;

}

