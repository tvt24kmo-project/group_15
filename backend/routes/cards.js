const express = require('express');
const router = express.Router();
const cards = require('../models/cards_model');

router.get('/', function (req, res) {
    cards.getAllCards(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

router.get('/:cardnumber', function (req, res) {
    cards.getByCardnumber(req.params.cardnumber, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


module.exports = router;