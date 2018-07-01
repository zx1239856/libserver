-- phpMyAdmin SQL Dump
-- version 4.7.5
-- https://www.phpmyadmin.net/
--
-- Host: localhost
-- Generation Time: 2018-07-01 16:43:31
-- 服务器版本： 5.7.20-log
-- PHP Version: 7.1.14

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `libserver`
--

-- --------------------------------------------------------

--
-- 表的结构 `lib_bookcase`
--

CREATE TABLE `lib_bookcase` (
  `ID` bigint(20) NOT NULL,
  `area` varchar(50) NOT NULL,
  `type` varchar(30) NOT NULL,
  `description` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_bookgroup`
--

CREATE TABLE `lib_bookgroup` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(20) NOT NULL,
  `max_time` int(10) UNSIGNED NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_bookMark`
--

CREATE TABLE `lib_bookMark` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `readerID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `ISBN` varchar(20) NOT NULL,
  `bookMark` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_books`
--

CREATE TABLE `lib_books` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(50) DEFAULT NULL,
  `groupID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `author` varchar(50) NOT NULL,
  `press` varchar(50) DEFAULT NULL,
  `tags` text,
  `ISBN` varchar(20) NOT NULL,
  `price` float UNSIGNED NOT NULL DEFAULT '0',
  `pages` smallint(5) UNSIGNED NOT NULL DEFAULT '0',
  `bookcase` bigint(20) NOT NULL,
  `inTime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `operatorID` bigint(20) NOT NULL,
  `available` tinyint(1) DEFAULT '1',
  `cover` varchar(40) DEFAULT NULL,
  `pdf` varchar(40) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_currappoint`
--

CREATE TABLE `lib_currappoint` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `type` enum('borrow','return') NOT NULL,
  `readerid` int(11) NOT NULL,
  `bookid` int(11) DEFAULT NULL,
  `appointtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `borrowid` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_currborrow`
--

CREATE TABLE `lib_currborrow` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `readerid` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `bookid` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `borrowtime` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `exptime` timestamp NOT NULL,
  `remaintime` int(20) NOT NULL,
  `operatorid` bigint(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_operation_log`
--

CREATE TABLE `lib_operation_log` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `type` varchar(20) NOT NULL,
  `content` text NOT NULL,
  `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `operatorID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `operatorType` enum('staff','reader','other') NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_prevBorrow`
--

CREATE TABLE `lib_prevBorrow` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `readerID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `bookID` int(10) UNSIGNED ZEROFILL NOT NULL,
  `borrowTime` timestamp NOT NULL,
  `expTime` timestamp NOT NULL,
  `operatorID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `overdueTime` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_readergroup`
--

CREATE TABLE `lib_readergroup` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` char(30) NOT NULL,
  `max_borrow_num` int(11) NOT NULL,
  `max_borrow_time` int(11) NOT NULL,
  `max_renew_time` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_readers`
--

CREATE TABLE `lib_readers` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `username` varchar(30) NOT NULL,
  `password` varchar(40) NOT NULL,
  `groupid` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(50) NOT NULL,
  `sex` enum('male','female','other','') NOT NULL,
  `tel` varchar(20) NOT NULL,
  `email` varchar(50) NOT NULL,
  `createDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `expDate` timestamp NULL DEFAULT NULL,
  `remark` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_settings`
--

CREATE TABLE `lib_settings` (
  `variable` varchar(128) NOT NULL,
  `value` varchar(128) NOT NULL,
  `set_time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `set_by` bigint(20) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- 转存表中的数据 `lib_settings`
--

INSERT INTO `lib_settings` (`variable`, `value`, `set_time`, `set_by`) VALUES
('logo', '', '2017-12-31 16:00:00', NULL),
('pdfWatermark', '', '2017-12-31 16:00:00', NULL);

-- --------------------------------------------------------

--
-- 表的结构 `lib_staffgroup`
--

CREATE TABLE `lib_staffgroup` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(20) NOT NULL,
  `privileges` tinyint(1) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

-- --------------------------------------------------------

--
-- 表的结构 `lib_staffs`
--

CREATE TABLE `lib_staffs` (
  `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `username` varchar(30) NOT NULL,
  `password` varchar(30) NOT NULL,
  `groupID` bigint(20) UNSIGNED ZEROFILL NOT NULL,
  `name` varchar(50) NOT NULL,
  `sex` enum('male','female','other','') NOT NULL,
  `tel` varchar(20) NOT NULL,
  `email` varchar(50) NOT NULL,
  `createDate` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `expDate` timestamp NOT NULL,
  `remark` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

--
-- Indexes for dumped tables
--

--
-- Indexes for table `lib_bookcase`
--
ALTER TABLE `lib_bookcase`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_bookgroup`
--
ALTER TABLE `lib_bookgroup`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_bookMark`
--
ALTER TABLE `lib_bookMark`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `readerID` (`readerID`),
  ADD KEY `ISBN` (`ISBN`);

--
-- Indexes for table `lib_books`
--
ALTER TABLE `lib_books`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_currappoint`
--
ALTER TABLE `lib_currappoint`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_currborrow`
--
ALTER TABLE `lib_currborrow`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_operation_log`
--
ALTER TABLE `lib_operation_log`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_prevBorrow`
--
ALTER TABLE `lib_prevBorrow`
  ADD PRIMARY KEY (`ID`),
  ADD KEY `readerID` (`readerID`),
  ADD KEY `bookID` (`bookID`),
  ADD KEY `operatorID` (`operatorID`);

--
-- Indexes for table `lib_readergroup`
--
ALTER TABLE `lib_readergroup`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_readers`
--
ALTER TABLE `lib_readers`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_settings`
--
ALTER TABLE `lib_settings`
  ADD PRIMARY KEY (`variable`);

--
-- Indexes for table `lib_staffgroup`
--
ALTER TABLE `lib_staffgroup`
  ADD PRIMARY KEY (`ID`);

--
-- Indexes for table `lib_staffs`
--
ALTER TABLE `lib_staffs`
  ADD PRIMARY KEY (`ID`);

--
-- 在导出的表使用AUTO_INCREMENT
--

--
-- 使用表AUTO_INCREMENT `lib_bookcase`
--
ALTER TABLE `lib_bookcase`
  MODIFY `ID` bigint(20) NOT NULL AUTO_INCREMENT;

--
-- 使用表AUTO_INCREMENT `lib_bookgroup`
--
ALTER TABLE `lib_bookgroup`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;

--
-- 使用表AUTO_INCREMENT `lib_bookMark`
--
ALTER TABLE `lib_bookMark`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT;

--
-- 使用表AUTO_INCREMENT `lib_books`
--
ALTER TABLE `lib_books`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=13;

--
-- 使用表AUTO_INCREMENT `lib_currappoint`
--
ALTER TABLE `lib_currappoint`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=31693;

--
-- 使用表AUTO_INCREMENT `lib_currborrow`
--
ALTER TABLE `lib_currborrow`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;

--
-- 使用表AUTO_INCREMENT `lib_operation_log`
--
ALTER TABLE `lib_operation_log`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=113;

--
-- 使用表AUTO_INCREMENT `lib_prevBorrow`
--
ALTER TABLE `lib_prevBorrow`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT;

--
-- 使用表AUTO_INCREMENT `lib_readergroup`
--
ALTER TABLE `lib_readergroup`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT;

--
-- 使用表AUTO_INCREMENT `lib_readers`
--
ALTER TABLE `lib_readers`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=4;

--
-- 使用表AUTO_INCREMENT `lib_staffgroup`
--
ALTER TABLE `lib_staffgroup`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT;

--
-- 使用表AUTO_INCREMENT `lib_staffs`
--
ALTER TABLE `lib_staffs`
  MODIFY `ID` bigint(20) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=2;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
