from flask import Flask
from flask import request
app=Flask(__name__)
global s
s='-1'
@app.route('/get_information')
def get_information():
    global s
    with open('c://data.txt','r') as f:
        returnStr=str(f.readline())
        with open('c://data.txt','w') as wf:
            wf.write(s)
            if s=='-1':
                s=returnStr
            else:
                s='-1'
        return returnStr
app.run(host='0.0.0.0')