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
// Listaa tilin tiedot kortinnumeron perusteella
router.get('/:uname', function(request, response){
    accounts.getById(request.params.uname,function(err, result){
        if(err){
            response.json(err);
        }
        else {
            response.json(result); // palautetaan KOKO TAULUKKO, KOSKA MULTI KORTTI EI SITTEN LÖYDÄ TILEJÄ
        }
    })
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


router.get('/fullname/:id', function (req, res) {
    accounts.getFullName(req.params.id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

module.exports = router;