#include "hwlib.hpp"
#include "mpu6050.hpp"

int main(){	  
   namespace target = hwlib::target;

   auto scl = target::pin_oc( target::pins::scl );
   auto sda = target::pin_oc( target::pins::sda );
   
   auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );

   mpu6050 chip(i2c_bus);
   chip.disableSleep();
   chip.accel_config(2);
   for(;;){
       chip.return_formatted_data();
   }
   
}
