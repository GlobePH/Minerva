import pyrebase
import csv


config = {
  "apiKey": "AIzaSyC1_TnydpQ5KkUCiqLF93lbplAldmOkac4",
  "authDomain": "project-minerva-e8729.firebaseapp.com",
  "databaseURL": "https://project-minerva-e8729.firebaseio.com/",
  "storageBucket": "gs://project-minerva-e8729.appspot.com/",
  "serviceAccount": "C:\\Users\Admin\Desktop\AI\Project-Minerva-abdc0b655a93.json"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()
db.child("SensorData")
db.child("MLData")                                
with open('ToBePredictedRAW.csv') as csvfile:
    read = csv.reader(csvfile, delimiter=",")
    for row in read:
        Humidity = {row[2]: row[3]}
        db.child("SensorData").child("Humidity").child(row[2]).set(Humidity)
        AirTemp = {"data": row[4]}
        db.child("SensorData").child("AirTemp").child(row[2]).set(AirTemp)
        WaterTemp = {"data": row[5]}
        db.child("SensorData").child("WaterTemp").child(row[2]).set(WaterTemp)
        WaterPH = {"data": row[6]}
        db.child("SensorData").child("WaterPH").child(row[2]).set(WaterPH)
        LightIntensity = {"data": row[7]}
        db.child("SensorData").child("LightIntensity").child(row[2]).set(LightIntensity)
        AtmCO2 = {"data": row[8]}
        db.child("SensorData").child("AtmCO2").child(row[2]).set(AtmCO2)

with open('YieldDecision.csv') as csvfile:
    read2 = csv.reader(csvfile, delimiter=",")
    for row1 in read2:
        YieldForecast = {row[2]:row1[0]}
        db.child("MLData").child("YieldForecast").set(YieldForecast)

with open('PumpDecision.csv') as csvfile:
    read3 = csv.reader(csvfile, delimiter=",")
    for row2 in read3:
        PumpStatus = {row[2]:row2[0]}
        db.child("MLData").child("PumpStatus").set(PumpStatus)
            

                 
