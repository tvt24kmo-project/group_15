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


router.put("/update-card-attempts/:idcard", function (req, res) {
    cards.updateWrongAttempt(req.body.wrong_attempts, req.params.idcard, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


router.post('/testLocking/:idcard', function (req, res) {
    const attempts = req.body.attempts;
    if (attempts === undefined) {
      return res.status(400).json({ error: 'No attempts provided' });
    }
  
    cards.updateWrongAttempt(attempts, req.params.idcard, function (err, result) {
      if (err) {
        return res.json(err);
      }
  
      if (attempts >= 3) {
        // Lock the card if attempts >= 3
        const db = require('../database'); // or wherever db is imported
        db.query(
          "UPDATE cards SET islocked=1 WHERE idcard=?",
          [req.params.idcard],
          function (errLock, lockResult) {
            if (errLock) {
              return res.json(errLock);
            }
            res.json({ message: 'Card locked', result: lockResult });
          }
        );
      } else {
        res.json({ message: 'Attempts updated', result });
      }
    });
  });



router.post("/update-card-attempts/:idcard", function (req, res) {
    cards.updateWrongAttempt(req.body.wrong_attempts, req.params.idcard, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


module.exports = router;    