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

//päivittää kortin id:n perusteella (pitäiskö vaihtaa kortin numeron perusteiseksi?)
router.put('/:idcard', function (req, res) {
    cards.update(req.params.idcard, req.body, function (err, result) {
        if(err){
            res.json(err);
        } else {
            res.json(result);
        }
    });
});        

//poistaa kortin id:n perusteella (pitäiskö taas vaihtaa kortin numeron perusteiseksi?)
router.delete('/:idcard', function (req, res) {
    cards.delete(req.params.idcard, function (err, result) {
        if(err){
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


router.put("/update-card-attempts/:cardnumber", function (req, res) {
    cards.updateWrongAttempt(req.body.wrong_attempts, req.params.cardnumber, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

router.get("/check-lock-status/:cardnumber", function (req, res) {
    cards.checkLockStatus(req.params.cardnumber, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});



module.exports = router;    