from flask import Flask 
from flask import render_template
from flask import request
from flask import redirect
from flask import url_for
from flask import flash
import requests
import pymongo


#recive data from esp and dispaly on web page
app = Flask(__name__)

app.secret_key = 'abc'
# client = pymongo.MongoClient("mongodb+srv://harshnishad:harshnishad@cluster0.llahgwx.mongodb.net/test")
# db = client["context"]
# keys=db["chat_id"]


def send_alert(humidity,gas,flame,temp,chat_id = 755899157):
    token = "6809491389:AAEa6X5MG_00wtiap1R2OGu-Zg5Mx2dlpMw"
    

    alert_msg=f"🔥🔥Fire alert🔥🔥\n \nTemperature: {temp}°C\nHumidity: {humidity}%\nGas: {gas}ppm\nFlame: {flame}"


    url = f"https://api.telegram.org/bot{token}/sendMessage"
    params = {
       "chat_id": chat_id,
       "text": alert_msg
    }
    resp = requests.get(url, params=params)

    # Throw an exception if Telegram API fails
    resp.raise_for_status()

@app.route("/sensordata/",methods=["POST","GET"])
def data():

    if request.method=="POST":
        # mongoid=(request.form.get('mongoid'))
        temp_s=(request.form.get('temp_s'))
        humidity_s=(request.form.get('humidity_s'))
        gas_s=(request.form.get('gas_s'))
        flame_s=(request.form.get('fire_s'))
        temp=(request.form.get('temp'))
        humidity=(request.form.get('humidity'))
        gas=(request.form.get('gas'))
        flame=(request.form.get('fire'))
        # alert=(request.form.get('alert'))
        # print(temp,flame,gas,alert)

        
        #convert string to int
        temp=float(temp)
        humidity=float(humidity)
        gas=float(gas)
        flame=float(flame)
        
        if flame < 3000 or temp > 33 or humidity > 80:
            send_alert(humidity,gas,flame,temp)

        print(humidity,gas,flame,temp)
        return "data recieved"
    
    return "no data recieved"


if __name__ == '__main__':  
    app.run(host='0.0.0.0',port=5000,debug = True)