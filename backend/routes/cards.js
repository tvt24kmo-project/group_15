const express = require('express');
const router = express.Router();
const cards = require('../models/cards_model');

//näytä kaikki tiedot kortista
router.get('/', function (req, res) {
    cards.getAllCards(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

//näytä kortti korttinumeron perusteella
router.get('/:cardnumber', function (req, res) {
    cards.getByCardnumber(req.params.cardnumber, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});
//lisää kortti, kryptaa salasanan
router.post('/', function (req, res) {
    cards.add(req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

router.put('/:idcard', function (req, res) {
    cards.update(req.params.idcard, req.body, function (err, result) {
        if(err){
            res.json(err);
        } else {
            res.json(result);
        }
    });
});        

module.exports = router;