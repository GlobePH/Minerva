import gspread
import csv
import pandas as pd
from oauth2client.service_account import ServiceAccountCredentials
# use creds to create a client to interact with the Google Drive API
scope = ['https://spreadsheets.google.com/feeds']
creds = ServiceAccountCredentials.from_json_keyfile_name('GDrive-e45fd21472a5.json', scope)
client = gspread.authorize(creds)
# Find a workbook by name and open the first sheet
# Make sure you use the right name here.
spreadsheet = client.open_by_key('12sXBG4si3KX4EwNwhLfEgc1uEKXMyuN3x8zirQirSdQ')
for i, worksheet in enumerate(spreadsheet.worksheets()):
    filename = 'docid' + '-worksheet' + str(i) + '.csv'
    with open('ToBePredictedRAW.csv', 'w') as f:
        writer = csv.writer(f)
        writer.writerows(worksheet.get_all_values())

cf = pd.read_csv('TobePredictedRAW.csv', skiprows=0,usecols=[0,1,2,3,4,5,6,7,8,9,10])
cf.to_csv('ToBePredictedRAW.csv',index=False)
df = pd.read_csv('TobePredictedRAW.csv', skiprows=0,usecols=[3,4,5,6,7,8,9,10])
df.to_csv('Yield.csv',index=False)
bf = pd.read_csv('ToBePredictedRAW.csv', skiprows=0,usecols=[3,4,5,6,7,8,9])
bf.to_csv('Pump.csv',index=False)


