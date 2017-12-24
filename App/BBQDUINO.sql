-- phpMyAdmin SQL Dump
-- version 4.6.6deb4
-- https://www.phpmyadmin.net/
--
-- Client :  localhost:3306
-- Généré le :  Dim 24 Décembre 2017 à 10:52
-- Version du serveur :  10.1.26-MariaDB-0+deb9u1
-- Version de PHP :  7.0.19-1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données :  `BBQDUINO`
--
CREATE DATABASE IF NOT EXISTS `BBQDUINO` DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;
USE `BBQDUINO`;

-- --------------------------------------------------------

--
-- Structure de la table `bbq`
--

DROP TABLE IF EXISTS `bbq`;
CREATE TABLE `bbq` (
  `ID` int(11) NOT NULL,
  `NAME` mediumtext NOT NULL,
  `DATEMAJ` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Contenu de la table `bbq`
--

INSERT INTO `bbq` (`ID`, `NAME`, `DATEMAJ`) VALUES
(1, 'Weber Genesis II E-310', '2017-11-05'),
(2, 'Weber Summit Charcoal', '2017-12-02'),
(3, 'Rejilla Arrosticini Miami', '2017-12-02');

-- --------------------------------------------------------

--
-- Structure de la table `meat`
--

DROP TABLE IF EXISTS `meat`;
CREATE TABLE `meat` (
  `ID` int(11) NOT NULL,
  `NAME` mediumtext NOT NULL,
  `DATEMAJ` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Contenu de la table `meat`
--

INSERT INTO `meat` (`ID`, `NAME`, `DATEMAJ`) VALUES
(1, 'Ribs', '2017-11-18'),
(2, 'Pulled Pork', '2017-11-18'),
(5, 'Brisket', '2017-11-18'),
(6, 'Saumon sur planche de cèdre', '2017-11-18'),
(7, 'Poulet', '2017-11-18'),
(8, 'Dinde', '2017-12-02');

-- --------------------------------------------------------

--
-- Structure de la table `sensor`
--

DROP TABLE IF EXISTS `sensor`;
CREATE TABLE `sensor` (
  `ID` int(11) NOT NULL,
  `NAME` varchar(255) NOT NULL,
  `DATEMAJ` date NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='Table des sondes de température';

--
-- Contenu de la table `sensor`
--

INSERT INTO `sensor` (`ID`, `NAME`, `DATEMAJ`) VALUES
(1, 'ambient temp sensor', '2017-09-29'),
(2, 'bbq temp sensor', '2017-11-01'),
(3, 'food temp sensor', '2017-11-01');

-- --------------------------------------------------------

--
-- Structure de la table `session`
--

DROP TABLE IF EXISTS `session`;
CREATE TABLE `session` (
  `ID` int(11) NOT NULL,
  `SESSION_DATE` date DEFAULT NULL,
  `SESSION_TIME` time DEFAULT NULL,
  `SESSION_MONTH` varchar(2) DEFAULT NULL,
  `SESSION_TSTAMP` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `SESSION_YEAR` year(4) DEFAULT NULL,
  `DESCRIPTION` longtext,
  `MEAT_ID` int(11) DEFAULT NULL,
  `BBQ_TEMP` int(11) DEFAULT NULL,
  `FOOD_TEMP` int(11) DEFAULT NULL,
  `BBQ_ID` int(11) DEFAULT NULL,
  `END_SESSION_DATE` date NOT NULL,
  `END_SESSION_TSTAMP` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `END_SESSION_TIME` time NOT NULL,
  `END` int(11) NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Contenu de la table `session`
--

INSERT INTO `session` (`ID`, `SESSION_DATE`, `SESSION_TIME`, `SESSION_MONTH`, `SESSION_TSTAMP`, `SESSION_YEAR`, `DESCRIPTION`, `MEAT_ID`, `BBQ_TEMP`, `FOOD_TEMP`, `BBQ_ID`, `END_SESSION_DATE`, `END_SESSION_TSTAMP`, `END_SESSION_TIME`, `END`) VALUES
(2, '2017-11-30', '09:00:00', '11', '2017-11-30 17:00:00', 2017, 'Ribs 1kg', 1, 140, 90, 1, '2017-12-17', '2017-11-19 09:06:28', '00:00:00', 1),
(7, '2017-12-17', '18:48:25', '12', '2017-12-17 17:48:25', 2017, 'test', 1, 110, 56, 1, '2017-12-17', '2017-12-17 17:48:25', '18:48:25', 0);

-- --------------------------------------------------------

--
-- Structure de la table `temperature`
--

DROP TABLE IF EXISTS `temperature`;
CREATE TABLE `temperature` (
  `ID` int(11) NOT NULL COMMENT 'numéro de la mesure',
  `SENSOR_ID` int(11) NOT NULL COMMENT 'id de le sonde',
  `CELCIUS` float NOT NULL COMMENT 'relevé en celcius',
  `FAHRENHEIT` float NOT NULL COMMENT 'relevé en Fahrenheit',
  `DATE_TEMP` date NOT NULL COMMENT 'date du relevé',
  `TIME_TEMP` time NOT NULL COMMENT 'heure du relevé',
  `MOIS_TEMP` int(2) NOT NULL COMMENT 'mois du relevé',
  `YEAR_TEMP` year(4) NOT NULL COMMENT 'année du relevé',
  `TSTAMP` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT 'timestamp du relevé',
  `SESSION_ID` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 COMMENT='table des relevés de température';

--
-- Index pour les tables exportées
--

--
-- Index pour la table `bbq`
--
ALTER TABLE `bbq`
  ADD PRIMARY KEY (`ID`);

--
-- Index pour la table `meat`
--
ALTER TABLE `meat`
  ADD PRIMARY KEY (`ID`);

--
-- Index pour la table `sensor`
--
ALTER TABLE `sensor`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `ID` (`ID`);

--
-- Index pour la table `session`
--
ALTER TABLE `session`
  ADD PRIMARY KEY (`ID`);

--
-- Index pour la table `temperature`
--
ALTER TABLE `temperature`
  ADD PRIMARY KEY (`ID`),
  ADD UNIQUE KEY `ID` (`ID`);

--
-- AUTO_INCREMENT pour les tables exportées
--

--
-- AUTO_INCREMENT pour la table `bbq`
--
ALTER TABLE `bbq`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
--
-- AUTO_INCREMENT pour la table `meat`
--
ALTER TABLE `meat`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=9;
--
-- AUTO_INCREMENT pour la table `sensor`
--
ALTER TABLE `sensor`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;
--
-- AUTO_INCREMENT pour la table `session`
--
ALTER TABLE `session`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=8;
--
-- AUTO_INCREMENT pour la table `temperature`
--
ALTER TABLE `temperature`
  MODIFY `ID` int(11) NOT NULL AUTO_INCREMENT COMMENT 'numéro de la mesure', AUTO_INCREMENT=11035;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
