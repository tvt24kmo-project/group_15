const express = require('express');
const router = express.Router();
const transactions = require('../models/accounts_model');


// lisää uusi tili tietokantaan
router.post('/', function (req, res) {
    account.addAccount(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Listaa tilien tiedott
router.get('/', function (req, res) {
    account.getAllAccounts(req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// Päivitä tili id:n perusteella
router.put('/:id', function (req, res) {
    account.updateAccount(req.params.id, req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// Poista tili id:n perusteella
router.delete('/:id', function (req, res) {
    account.removeAccount(req.params.id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});



module.exports = router;