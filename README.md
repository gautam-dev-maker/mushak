# Mushak (Micromouse CAD model)
* Mushak is a half-size micromouse that solves 16x16 grid of maze with high performance.
<p align="center">
  <img src="./assets/Micromouse.PNG" width="700"/>
</p>

# Part Descriptions
* The bot contains a total of _ components which are as listed in the following table :


|     Parts             |          Description          |
| --------------------- | ----------------------------- |
| <p align="center">
  <img src="./assets/motor.png" width="200"/> | Motors - This is the motor we are using, which has </br> dimensions: Length x Dia. 12 x 6 mm and a shaft lenght of 6mm.|
  <img src="./assets/mount.png" width="200"/> | Motor_mount - This is the mount for the motors, which holds </br> the motor, shaft, gears, bearing and magnet.Because of less center-to-center distance between gears, after many iterations this mount was designed.|
  <img src="./assets/tof_mount.png" width="200"/> | TOF_mount - This is the mount for TOF sensor, which is placed </br> in front. We have used VL6180x TOF sensor.|  
   <img src="./assets/IR_mount.png" width="200"/> | IR_mounts - These are the mounts for IR sensors placed </br> at specific angles. SFH-3015-FA and SFH-4045N sensors are phototransistors and IR emitters respectively.|   
   <img src="./assets/gears.png" width="200"/> | Gears - The metal gears were extracted from mg90s servos having dimensions: </br> Driving gear:</br> &emsp; &emsp; &emsp;&emsp; &emsp;Teeth 9 </br> &emsp; &emsp; &emsp;&emsp; &emsp;Pressure Angle - 20º</br> &emsp; &emsp; &emsp;&emsp; &emsp;ID - 0.5, OD - 2.14 mm</br> &emsp; &emsp; &emsp;&emsp; &emsp;Width 2.46 mm</br> Driven gear: </br> &emsp; &emsp; &emsp;&emsp; &emsp;Teeth 47 </br> &emsp; &emsp; &emsp;&emsp; &emsp;Pressure Angle 20º</br> &emsp; &emsp; &emsp;&emsp; &emsp;ID - 1.5mm, OD - 9.55 mm</br> &emsp; &emsp; &emsp;&emsp; &emsp; Width 1.2 mm.</br> Center to center distance is 5.80mm and modulus 0.195</br>| 
   <img src="./assets/others_1.png" width="200"/> | Bearings, shaft and Wheels - These are the passive components used.</br> Bearings x4 of dimensions: ID x OD 1.5 x 4 mm.</br> Shaft x2 of dimensions: Length x Dia. 15 x 1.5 mm.</br> Wheels with dimensions: ID x OD 1.5 x 12.5 mm.</br> A combination of 2 bearings is used to house the shaft for better stability and lesser Vibrations.|
   || IC's and Battery - The IC's used in CAD model are with their actual dimensions.</br> Lipo battery of dimension: 1.5 x 2.5 mm.|
</p> 

  # Issues Faced in Mechanical Designing 
  |   Parts    |       Issues       |
  |------------|--------------------|
  |<p align="center"><img src="./assets/mount.png" width="200"/><img src="./assets/mount_assem.png" width="200"/><img src="./assets/mount_assem_transparent.png" width="200"/><img src="./assets/mount_gear.png" width="200"/>| Issue : The main issues faced in designing the Mount were  As the Gears acquired were of a smaller diameter, there was an issue of interference between the bearing and the motor. As they were too close, it was impossible to 3D print a mount with the thickness less than 1mm. </br> Solution : The solution opted for this was to actually disalign the motor and the shaft to give room the bearing housing to be #d printed. This enabled us to print a more stable base due to the stronger Shaft housing.|
  |<p align="center"><img src="./assets/gears.png" width="200"/>|Issue -  As there are a varieties of gears available. It was hard for us to choose the right combination and Built the Model and mounts around it.</br>  Solution- We started with finalising the sizes and the gear ratios needed according to the available spaces. We then did the market research to find the gears closest to our needs and incorporated those models into the Design.|
  # Additional Materials
  * Refer the following playlists to sharpen you skills in SolidWorks
  * [Tutorial](https://www.youtube.com/watch?v=Ulttc_2p4DY&list=PLrOFa8sDv6jcp8E3ayUFZ4iNI8uuPjXHe)
