-- MySQL dump 10.13  Distrib 8.0.40, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: atm_automat
-- ------------------------------------------------------
-- Server version	8.2.0

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `account_cards`
--

USE atm_automat;

DROP TABLE IF EXISTS `account_cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `account_cards` (
  `account_id` int NOT NULL,
  `card_id` int NOT NULL,
  PRIMARY KEY (`card_id`,`account_id`),
  KEY `fk_account_card_account_idx` (`account_id`),
  CONSTRAINT `fk_account_card_account` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`idaccount`) ON DELETE CASCADE,
  CONSTRAINT `fk_account_card_card` FOREIGN KEY (`card_id`) REFERENCES `cards` (`idcard`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `account_cards`
--

LOCK TABLES `account_cards` WRITE;
/*!40000 ALTER TABLE `account_cards` DISABLE KEYS */;
INSERT INTO `account_cards` VALUES (7,15),(9,16),(10,17),(11,17);
/*!40000 ALTER TABLE `account_cards` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `accounts`
--

DROP TABLE IF EXISTS `accounts`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `accounts` (
  `idaccount` int NOT NULL AUTO_INCREMENT,
  `customer_id` int NOT NULL,
  `balance` decimal(10,2) NOT NULL DEFAULT '0.00',
  `accountiban` varchar(18) NOT NULL,
  `credit_limit` decimal(10,2) DEFAULT '0.00',
  PRIMARY KEY (`idaccount`),
  UNIQUE KEY `accountiban_UNIQUE` (`accountiban`),
  KEY `fk_accounts_customer_id_idx` (`customer_id`),
  CONSTRAINT `fk_account_customer` FOREIGN KEY (`customer_id`) REFERENCES `customers` (`idcustomer`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accounts`
--

LOCK TABLES `accounts` WRITE;
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
INSERT INTO `accounts` VALUES (7,1,999069.98,'fi5566667777888898',1221312.00),(9,2,100000.00,'fi1122223333444455',0.00),(10,3,100000.00,'fi123456678912',10000.00),(11,3,98765.00,'fi9988887777666655',0.00);
/*!40000 ALTER TABLE `accounts` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cards`
--

DROP TABLE IF EXISTS `cards`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `cards` (
  `idcard` int NOT NULL AUTO_INCREMENT,
  `cardtype` enum('Debit','Credit','Multi') NOT NULL,
  `pinhash` varchar(255) NOT NULL,
  `islocked` tinyint(1) NOT NULL DEFAULT '0' COMMENT 'boolean arvo onko kortti lukossa (0/1)',
  `wrong_attempts` int NOT NULL DEFAULT '0',
  `cardnumber` varchar(16) NOT NULL,
  PRIMARY KEY (`idcard`),
  UNIQUE KEY `cardnumber_UNIQUE` (`cardnumber`)
) ENGINE=InnoDB AUTO_INCREMENT=18 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cards`
--

LOCK TABLES `cards` WRITE;
/*!40000 ALTER TABLE `cards` DISABLE KEYS */;
INSERT INTO `cards` VALUES (15,'Credit','$2a$10$uSUqClkJ6p8D9aSIxri1v.kffIX5DK9T0bbOfvL3fTPAV9WaVHinq',0,0,'1111222233334444'),(16,'Debit','$2a$10$.o1rHTcqI/vh.c69t5.Ma.O1sXQyRHG6io0xI.FxPkHchmdc8QXjK',0,0,'1212343456567878'),(17,'Multi','$2a$10$y./CAdmKeW/rZ8JtjSYhKO7jqg9nBYW.d8cLb7Xe5inwSy28uFRfG',0,0,'9999888877776666');
/*!40000 ALTER TABLE `cards` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `lock_card_after_wrong_attempts` BEFORE UPDATE ON `cards` FOR EACH ROW BEGIN
    IF NEW.wrong_attempts >= 3 THEN
        SET NEW.islocked = 1;
    END IF;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `customers`
--

DROP TABLE IF EXISTS `customers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `customers` (
  `idcustomer` int NOT NULL AUTO_INCREMENT,
  `fname` varchar(50) NOT NULL,
  `lname` varchar(50) NOT NULL,
  `address` varchar(45) NOT NULL,
  `city` varchar(45) DEFAULT NULL,
  `postalcode` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`idcustomer`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `customers`
--

LOCK TABLES `customers` WRITE;
/*!40000 ALTER TABLE `customers` DISABLE KEYS */;
INSERT INTO `customers` VALUES (1,'nm','ln','adrs','ct','pcd'),(2,'nimi','sukunimi','osoite','kaupunni','90420'),(3,'matti','mallikas','tietöntie','oulu','90100');
/*!40000 ALTER TABLE `customers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `transactions`
--

DROP TABLE IF EXISTS `transactions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `transactions` (
  `idtransactions` int NOT NULL AUTO_INCREMENT,
  `account_id` int NOT NULL,
  `amount` decimal(10,2) NOT NULL,
  `transaction_date` datetime DEFAULT CURRENT_TIMESTAMP,
  `transaction_type` enum('withdraw','deposit','transfer') NOT NULL,
  PRIMARY KEY (`idtransactions`),
  KEY `fk_transaction_card_idx` (`account_id`),
  CONSTRAINT `fk_transaction_card` FOREIGN KEY (`account_id`) REFERENCES `accounts` (`idaccount`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=80 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transactions`
--

LOCK TABLES `transactions` WRITE;
/*!40000 ALTER TABLE `transactions` DISABLE KEYS */;
INSERT INTO `transactions` VALUES (55,7,0.01,'2025-01-25 01:31:38','withdraw'),(56,7,0.01,'2025-01-28 00:59:15','withdraw'),(57,7,0.01,'2025-01-29 20:39:01','withdraw'),(58,7,20.00,'2025-01-29 23:05:07','withdraw'),(59,7,40.00,'2025-01-30 00:10:21','withdraw'),(60,7,20.00,'2025-01-30 00:15:42','withdraw'),(61,7,20.00,'2025-01-30 00:17:27','withdraw'),(62,7,20.00,'2025-01-30 00:19:45','withdraw'),(63,7,20.00,'2025-01-30 00:20:54','withdraw'),(64,7,20.00,'2025-01-30 00:21:32','withdraw'),(65,7,20.00,'2025-01-30 00:23:36','withdraw'),(66,7,20.00,'2025-01-30 00:25:22','withdraw'),(67,7,20.00,'2025-01-30 00:27:47','withdraw'),(68,7,40.00,'2025-01-30 00:29:30','withdraw'),(69,7,50.00,'2025-01-30 00:29:41','withdraw'),(70,7,120.00,'2025-01-30 00:29:46','withdraw'),(71,7,120.00,'2025-01-30 12:47:54','withdraw'),(72,7,70.00,'2025-01-30 12:47:57','withdraw'),(73,7,30.00,'2025-01-30 12:48:00','withdraw'),(74,7,100.00,'2025-01-30 12:48:19','withdraw'),(75,7,70.00,'2025-01-30 12:50:57','withdraw'),(76,7,60.00,'2025-01-30 12:51:01','withdraw'),(77,7,50.00,'2025-01-30 12:54:35','withdraw'),(79,9,20.00,'2025-01-30 16:31:26','withdraw');
/*!40000 ALTER TABLE `transactions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'atm_automat'
--

--
-- Dumping routines for database 'atm_automat'
--
/*!50003 DROP PROCEDURE IF EXISTS `GetAccountType` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetAccountType`(
    IN p_account_id INT,
    OUT p_account_type VARCHAR(10)
)
BEGIN
    DECLARE v_limit DECIMAL(15, 2);

    -- Fetch the credit limit for the account
    SELECT credit_limit INTO v_limit FROM accounts WHERE idaccount = p_account_id;

    IF v_limit IS NULL OR v_limit = 0 THEN
        SET p_account_type = 'debit';
    ELSE
        SET p_account_type = 'credit';
    END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `GetCustomerTransactions` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `GetCustomerTransactions`(
    IN p_customer_id INT,
    IN p_page INT
)
BEGIN
    DECLARE v_offset INT;
    
    -- Validate page input
    IF p_page < 1 THEN
        SET p_page = 1;
    END IF;
    
    -- Calculate pagination offset
    SET v_offset = (p_page - 1) * 10;
    
    -- Get formatted transactions for all customer's accounts
    SELECT 
        DATE_FORMAT(t.transaction_date, '%d.%m.%Y %H:%i:%s') AS aika,
        a.accountiban AS tilinumero,
        t.amount AS määrä,
        t.transaction_type AS tapahtuma
    FROM transactions t
    JOIN accounts a ON t.account_id = a.idaccount
    WHERE a.customer_id = p_customer_id
    ORDER BY t.idtransactions DESC
    LIMIT 10 OFFSET v_offset;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `WithdrawFunds` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` PROCEDURE `WithdrawFunds`(
    IN p_account_id INT,
    IN p_amount DECIMAL(10, 2),
    OUT p_status VARCHAR(50)
)
BEGIN
    DECLARE v_balance DECIMAL(10, 2);
    DECLARE v_limit DECIMAL(10, 2);

    -- Fetch account details
    SELECT balance, credit_limit INTO v_balance, v_limit FROM accounts WHERE idaccount = p_account_id;

    IF v_balance + IFNULL(v_limit, 0) >= p_amount THEN
        UPDATE accounts SET balance = balance - p_amount WHERE idaccount = p_account_id;
        INSERT INTO transactions (account_id, transaction_type, amount) VALUES (p_account_id, 'withdraw', p_amount);
        SET p_status = 'Success';
    ELSE
        SET p_status = 'Insufficient funds';
    END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-01-30 19:16:54
