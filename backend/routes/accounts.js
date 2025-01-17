const express = require('express');
const router = express.Router();
const accounts = require('../models/accounts_model');


// lisää uusi tili tietokantaan
router.post('/', function (req, res) {
    accounts.addAccount(req.body, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


// Listaa tilien tiedott
router.get('/', function (req, res) {
    accounts.getAllAccounts(function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// Päivitä tili id:n perusteella
router.put('/:id', function (req, res) {
    accounts.updateAccount(req.body, req.params.id,  function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

// Poista tili id:n perusteella
router.delete('/:id', function (req, res) {
    accounts.deleteAccount(req.params.id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});



module.exports = router;