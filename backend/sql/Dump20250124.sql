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
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `accounts`
--

LOCK TABLES `accounts` WRITE;
/*!40000 ALTER TABLE `accounts` DISABLE KEYS */;
INSERT INTO `accounts` VALUES (4,1,-20.07,'fi5566667777888899',100.00),(7,1,0.42,'fi5566667777888898',1221312.00),(8,1,0.00,'fi1122223333444455',0.00);
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
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cards`
--

LOCK TABLES `cards` WRITE;
/*!40000 ALTER TABLE `cards` DISABLE KEYS */;
INSERT INTO `cards` VALUES (2,'Credit','$2a$10$.3pNibML7TIse6TRSbWjB.YVfkIIJIcR8tNS.Nsz3Wg8DdsRaVUTK',1,3,'4290571512346596');
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
INSERT INTO `customers` VALUES (1,'nm','ln','adrs','ct','pcd'),(2,'a','b','c',NULL,NULL),(3,'a','b','c',NULL,NULL);
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
) ENGINE=InnoDB AUTO_INCREMENT=55 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `transactions`
--

LOCK TABLES `transactions` WRITE;
/*!40000 ALTER TABLE `transactions` DISABLE KEYS */;
INSERT INTO `transactions` VALUES (7,4,200.00,'2025-01-24 23:24:47','withdraw'),(8,4,0.01,'2025-01-24 23:38:15','withdraw'),(9,4,0.01,'2025-01-24 23:51:25','withdraw'),(10,4,0.01,'2025-01-24 23:51:33','withdraw'),(11,4,0.01,'2025-01-24 23:51:35','withdraw'),(12,4,0.01,'2025-01-24 23:51:35','withdraw'),(13,4,0.01,'2025-01-24 23:51:36','withdraw'),(14,4,0.01,'2025-01-24 23:51:37','withdraw'),(15,4,0.01,'2025-01-24 23:51:37','withdraw'),(16,4,0.01,'2025-01-24 23:51:38','withdraw'),(17,4,0.01,'2025-01-24 23:51:38','withdraw'),(18,4,0.01,'2025-01-24 23:51:39','withdraw'),(19,4,0.01,'2025-01-24 23:51:39','withdraw'),(20,4,0.01,'2025-01-24 23:51:40','withdraw'),(21,4,0.01,'2025-01-24 23:51:41','withdraw'),(22,4,0.01,'2025-01-24 23:51:45','withdraw'),(23,4,0.01,'2025-01-24 23:51:46','withdraw'),(24,4,0.01,'2025-01-24 23:51:47','withdraw'),(25,4,0.01,'2025-01-24 23:51:47','withdraw'),(26,4,0.01,'2025-01-24 23:51:48','withdraw'),(27,4,0.01,'2025-01-24 23:51:48','withdraw'),(28,4,0.01,'2025-01-24 23:51:48','withdraw'),(29,4,0.01,'2025-01-24 23:51:48','withdraw'),(30,4,0.01,'2025-01-24 23:51:48','withdraw'),(31,4,0.01,'2025-01-24 23:51:49','withdraw'),(32,4,0.01,'2025-01-24 23:53:00','withdraw'),(33,4,0.01,'2025-01-24 23:56:01','withdraw'),(34,4,0.01,'2025-01-24 23:56:14','withdraw'),(35,4,0.01,'2025-01-24 23:56:24','withdraw'),(36,4,0.01,'2025-01-24 23:56:25','withdraw'),(37,4,0.01,'2025-01-24 23:56:25','withdraw'),(38,4,0.01,'2025-01-24 23:56:25','withdraw'),(39,4,0.01,'2025-01-24 23:56:26','withdraw'),(40,4,0.01,'2025-01-24 23:56:26','withdraw'),(41,4,0.01,'2025-01-24 23:56:26','withdraw'),(42,4,0.01,'2025-01-25 00:17:31','withdraw'),(43,4,199.65,'2025-01-25 00:17:58','withdraw'),(44,4,0.01,'2025-01-25 00:18:05','withdraw'),(45,4,0.01,'2025-01-25 00:20:53','withdraw'),(46,4,0.01,'2025-01-25 00:22:40','withdraw'),(47,4,0.01,'2025-01-25 00:23:34','withdraw'),(48,4,0.01,'2025-01-25 00:25:26','withdraw'),(49,4,0.01,'2025-01-25 00:25:27','withdraw'),(50,4,0.01,'2025-01-25 00:25:27','withdraw'),(51,4,10.00,'2025-01-25 00:25:31','withdraw'),(52,4,10.00,'2025-01-25 00:25:32','withdraw'),(53,8,10.00,'2025-01-25 00:28:23','withdraw'),(54,8,100.00,'2025-01-25 00:29:15','withdraw');
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

-- Dump completed on 2025-01-25  0:56:44
