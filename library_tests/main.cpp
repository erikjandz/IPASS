#include "mpu6050.hpp"
#include "hwlib.hpp"

int main(){

    namespace target = hwlib::target;

    auto scl = target::pin_oc( target::pins::scl );
    auto sda = target::pin_oc( target::pins::sda );
   
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( scl,sda );

    mpu6050 chip(i2c_bus);

    unsigned int passed_test_count = 0;
    
    //test case 1
    //test if you get data when the chip is not in sleep mode
    chip.disableSleep();
    int counter = 0;
    for(unsigned int i = 0; i < 10; i ++){
        counter += chip.get_position_x();
        counter += chip.get_position_y();
        counter += chip.get_position_z();
        counter += chip.get_gyro_x();
        counter += chip.get_gyro_y();
        counter += chip.get_gyro_z();
    }
    //if it didn't just return 0
    if(counter != 0){
        passed_test_count ++;
        hwlib::cout << "test case 1 passed" << hwlib::endl;
    }

    //test case 2
    //test if the chip will only return 0 over a various amount of measurements when it is in sleep mode 
    chip.enableSleep();
    counter = 0;
    for(unsigned int i = 0; i < 10; i ++){
        counter += chip.get_position_x();
        counter += chip.get_position_y();
        counter += chip.get_position_z();
        counter += chip.get_gyro_x();
        counter += chip.get_gyro_y();
        counter += chip.get_gyro_z();
    }
    //if it only returned 0
    if(counter == 0){
        passed_test_count ++;
        hwlib::cout << "test case 2 passed" << hwlib::endl;
    }
    
    //test case 3
    //test if you get data when disabling and enabling sleep mode multiple times
    chip.disableSleep(); 
    chip.enableSleep();
    chip.disableSleep();
    chip.enableSleep();
    chip.disableSleep();
    counter = 0;
    for(unsigned int i = 0; i < 10; i ++){
        counter += chip.get_position_x();
        counter += chip.get_position_y();
        counter += chip.get_position_z();
        counter += chip.get_gyro_x();
        counter += chip.get_gyro_y();
        counter += chip.get_gyro_z();
    }
    
    //if it didn't just return 0
    if(counter != 0){
        passed_test_count ++;
        hwlib::cout << "test case 3 passed" << hwlib::endl;
    }
    
    //test case 4
    //test if temperature in celcius is the same as the temperature in kelvin and fahrenehit
    chip.disableSleep();
    int celcius;
    int kelvin;
    int fahrenheit;
    counter = 0;
    for(unsigned int i = 0; i < 10; i ++){
        celcius = chip.get_temperature_in_C();
        kelvin = chip.get_temperature_in_K();
        fahrenheit = chip.get_temperature_in_F();

        if((celcius + 273 > kelvin * 0.8 && celcius + 273 < kelvin * 1.2) && (1.32* fahrenheit >= celcius * 1.8 + 32 && 0.8 * fahrenheit <= celcius * 1.8 + 32)){
            counter ++;
        }
    }
    
    //test if 90% of calculations are right
    if(counter >= 9){
        passed_test_count ++;
        hwlib::cout << "test case 4 passed" << hwlib::endl;
    }
    
    //test case 5
    //test if the measurements of the accelerometer won't exceed -90 or 90
    chip.disableSleep();
    counter = 0;
    for(unsigned int i = 0; i < 10; i ++){
        if(chip.get_position_x() > 90 || chip.get_position_y() > 90 || chip.get_position_z() > 90){
            if(chip.get_position_x() < -90 || chip.get_position_y() < -90 || chip.get_position_z() < -90){
                counter ++;
            }
        }
    }
    //test if it never exceeds the max
    if(counter == 0){
        passed_test_count ++;
        hwlib::cout << "test case 5 passed" << hwlib::endl;

    }
    
    //test case 6
    //test if freeze mode will return the same data over and over
    chip.disableSleep();
    chip.freeze();
    counter = 0;
    int counter1;
    int counter2;
    for(unsigned int i = 0; i < 11; i ++){
        counter2 = 0;
        counter2 += chip.get_position_x();
        counter2 += chip.get_position_y();
        counter2 += chip.get_position_z();
        counter2 += chip.get_gyro_x();
        counter2 += chip.get_gyro_y();
        counter2 += chip.get_gyro_z();
        if(counter2 == counter1){
            counter ++;
        }
        counter1 = counter2;
    }
    
    //if the amount of tests passed is 90 %
    if(counter >= 10){
        passed_test_count ++;
        hwlib::cout << "test case 6 passed" << hwlib::endl;
    }
    
    //test case 7
    //test if different scale_range values will return similiar data
    chip.disableSleep();
    int array[4] = {0,0,0,0};
    for( unsigned int j = 0; j < 4; j++){
        chip.accel_config(j);
        for(unsigned int i = 0; i < 20; i++){
            array[j] += chip.get_position_x();
            array[j] += chip.get_position_y();
            array[j] += chip.get_position_z();
        }
        array[j] *= -1;
    }
    
    if(array[0] * 0.7 < array[1] && array[0] * 1.3 > array[1]){
        if(array[0] * 0.7 < array[2] && array[0] * 1.3 > array[2]){
            if(array[0] * 0.7 < array[3] && array[0] * 1.3 > array[3]){
                passed_test_count ++;
                hwlib::cout << "test case 7 passed" << hwlib::endl;
            }           
        }
    }
    
    if(passed_test_count - 6){
        hwlib::cout << "all tests passed";
    }else{
        hwlib::cout << "only " << passed_test_count << " tests passed";
    }
    
}