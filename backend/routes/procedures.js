const express = require('express');
const router = express.Router();
const procedures = require('../models/procedures_model');

router.post('/withdraw', function (req, res) {
    procedures.withdraw(req.body.account_id, req.body.amount, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});

router.post('/getAccountType', function (req, res) {
    procedures.getAccountType(req.body.account_id, function (err, result) {
        if (err) {
            res.json(err);
        } else {
            res.json(result);
        }
    });
});


module.exports = router;