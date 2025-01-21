const express = require('express');
const router = express.Router();
const transactions = require('../models/transactions_model');
const accounts = require('../models/accounts_model');


// Hae kaikki tapahtumat
router.get('/', function (req, res) {
    transactions.getAllTransactions(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Lisää uusi tapahtuma tietokantaan
router.post('/', function (req, res) {
    transactions.addTransaction(req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Poista tapahtuma id:n perusteella
router.delete('/:id', function (req, res) {
    transactions.removeTransaction(req.params.id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Päivitä tapahtuma id:n perusteella
router.put('/:id', function (req, res) {
    transactions.updateTransaction(req.params.id, req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// näytä historia transactions.accountiban perusteella LISÄMUOTOILUA KUN FRONTEND TIEDOSSA
router.get('/:id', function (req, res) {
    transactions.getIbanHistory(req.params.id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

module.exports = router;