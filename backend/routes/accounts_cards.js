const express = require('express');
const router = express.Router();
const accounts_cards = require('../models/accounts_cards_model');


router.post('/', function (req, res) {
    accounts_cards.addAC(req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

router.get('/', function (req, res) {
    accounts_cards.getAllAC(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// hae accountidn perusteella
router.get('/account/:accountId', (req, res) => {
    accounts_cards.getByAccountId(req.params.accountId, (err, result) => {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// hae cardidn perusteella
router.get('/card/:cardId', (req, res) => {
    accounts_cards.getByCardId(req.params.cardId, (err, result) => {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// hae molempien perusteella
router.get('/account/:accountId/card/:cardId', (req, res) => {
  accounts_cards.getByBothIds(req.params.accountId, req.params.cardId, (err, result) => {
        if (err) {
          res.json(err);
        } else {
          res.json(result);
        }
    });
});

// päivitä accountidn ja cardidn perusteella
router.put('/account/:accountId/card/:cardId', function (req, res) {
    accounts_cards.updateAC(req.params.accountId, req.params.cardId, req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Poista accountidn ja cardidn perusteella
router.delete('/account/:accountId/card/:cardId', function (req, res) {
    accounts_cards.deleteAC(req.params.accountId, req.params.cardId, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// Poista kaikki accountidn perusteella
router.delete('/account/:accountId', function(req, res) {
  accounts_cards.deleteByAccountId(req.params.accountId, function (err, result) {
    if (err) {
      res.json(err);
    } else {
      res.json(result);
    }
  });
});

// Poista kaikki cardidn perusteella
router.delete('/card/:cardId', function(req, res) {
  accounts_cards.deleteByCardId(req.params.cardId, function(err, result) {
    if (err) {
        res.json(err);
    } else {
      res.json(result);
    }
  });
});



module.exports = router;