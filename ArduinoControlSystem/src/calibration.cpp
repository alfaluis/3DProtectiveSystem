#include <Arduino.h>

#define MQ2RL (9.47)
#define MQ2R0FACTOR (9.83)

#define CALIBARAION_SAMPLE_TIME     50
#define CALIBRATION_SAMPLE_INTERVAL 500
#define READ_SAMPLE_INTERVAL 50
#define READ_SAMPLE_TIMES    5

/*****************************Globals***********************************************/
// CO es monoxido de carbono
float    COCurve[3]  = {2.3,0.72,-0.34}; //data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15) 
float SmokeCurve[3] = {2.3,0.53,-0.44};   //data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)                                                     


/****************** MQResistanceCalculation ****************************************
Input:   raw_adc - raw value read from adc, which represents the voltage
Output:  the calculated sensor resistance
Remarks: The sensor and the load resistor forms a voltage divider. Given the voltage
         across the load resistor and its resistance, the resistance of the sensor
         could be derived.
************************************************************************************/ 
float MQResistanceCalculation(int raw_adc){
  return (float)MQ2RL * (1023 - raw_adc) / raw_adc;
}

/***************************** MQCalibration ****************************************
Input:   mq_pin - analog channel
Output:  Ro of the sensor
Remarks: This function assumes that the sensor is in clean air.
************************************************************************************/ 
float MQCalibration(int mq_pin){
  int i;
  float val=0;

  for (i = 0;i < CALIBARAION_SAMPLE_TIME; i++) {            //take multiple samples
    val += MQResistanceCalculation(analogRead(mq_pin));
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val / CALIBARAION_SAMPLE_TIME;                   //calculate the average value
  val = val / MQ2R0FACTOR;                        //divided by RO_CLEAN_AIR_FACTOR yields the Ro 

  return val; 
}
/*****************************  MQRead *********************************************
Input:   mq_pin - analog channel
Output:  Rs of the sensor
Remarks: This function use MQResistanceCalculation to caculate the sensor resistenc (Rs).
************************************************************************************/ 
float MQRead(int mq_pin)
{
  int i;
  float rs=0;

  for (i=0;i<READ_SAMPLE_TIMES;i++) {
    rs += MQResistanceCalculation(analogRead(mq_pin));
    delay(READ_SAMPLE_INTERVAL);
  }

  rs = rs/READ_SAMPLE_TIMES;

  return rs;  
}

/*****************************  MQGetPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         pcurve      - pointer to the curve of the target gas
Output:  ppm of the target gas
Remarks: By using the slope and a point of the line. The x(logarithmic value of ppm) 
         of the line could be derived if y(rs_ro_ratio) is provided. 
************************************************************************************/ 
int  MQGetPercentage(float rs_ro_ratio, float *pcurve){
  return pow(10, (log(rs_ro_ratio) - pcurve[1]) / pcurve[2] + pcurve[0]);
}

/*****************************  MQGetGasPercentage **********************************
Input:   rs_ro_ratio - Rs divided by Ro
         gas_id      - target gas type
Output:  ppm of the target gas
Remarks: This function passes different curves to the MQGetPercentage function which 
         calculates the ppm (parts per million) of the target gas.
************************************************************************************/ 
int MQGetGasPercentage(float rs_ro_ratio, char * gas_id){
  if (strcmp (gas_id,"CO") == 0) {
     return MQGetPercentage(rs_ro_ratio, COCurve);
  } else if (strcmp (gas_id,"SMOKE") == 0) {
     return MQGetPercentage(rs_ro_ratio, SmokeCurve);
  }
  return 0;
}