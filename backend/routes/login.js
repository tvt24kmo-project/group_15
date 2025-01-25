const express = require('express');
const router = express.Router();
const bcrypt = require('bcryptjs');
const cards = require('../models/cards_model');
const jwt = require('jsonwebtoken');
const dotenv = require('dotenv');

//login controller 

router.post('/', 
  function(request, response) {
    if(request.body.cardnumber && request.body.pinhash){
      const user = request.body.cardnumber;
      const pass = request.body.pinhash;
      
        cards.checkPassword(user, function(dbError, dbResult) {
          if(dbError){
            response.json("-11");
          }
          else{
            if (dbResult.length > 0) {
              bcrypt.compare(pass,dbResult[0].pinhash, function(err,compareResult) {
                if(compareResult) {
                  console.log("success");
                  const token = generateAccessToken({ cardnumber: user });
                  response.send(token);
                }
                else {
                    console.log("wrong password");
                    response.send(false);
                }			
              }
              );
            }
            else{
              console.log("user does not exists");
              response.send(false);
            }
          }
          }
        );
      }
    else{
      console.log("username or password missing");
      response.send(false);
    }
  }
);

function generateAccessToken(cardnumber) {
  dotenv.config();
  return jwt.sign(cardnumber, process.env.MY_TOKEN, { expiresIn: '1800s' });
}

module.exports=router;