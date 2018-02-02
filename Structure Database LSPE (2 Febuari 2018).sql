-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL,ALLOW_INVALID_DATES';

-- -----------------------------------------------------
-- Schema LSPE
-- -----------------------------------------------------
DROP SCHEMA IF EXISTS `LSPE` ;

-- -----------------------------------------------------
-- Schema LSPE
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `LSPE` DEFAULT CHARACTER SET utf8 ;
USE `LSPE` ;

-- -----------------------------------------------------
-- Table `LSPE`.`address_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`address_type` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`address_type` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`address`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`address` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`address` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `address` TEXT NOT NULL,
  `type_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_address_address_type1`
    FOREIGN KEY (`type_id`)
    REFERENCES `LSPE`.`address_type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_address_address_type1_idx` ON `LSPE`.`address` (`type_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`phone_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`phone_type` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`phone_type` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`phone`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`phone` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`phone` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `number` VARCHAR(45) NOT NULL,
  `type_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_phone_phone_type1`
    FOREIGN KEY (`type_id`)
    REFERENCES `LSPE`.`phone_type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_phone_phone_type1_idx` ON `LSPE`.`phone` (`type_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`number_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`number_type` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`number_type` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`number`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`number` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`number` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `number` VARCHAR(45) NOT NULL,
  `type_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_number_number_type1`
    FOREIGN KEY (`type_id`)
    REFERENCES `LSPE`.`number_type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_number_number_type1_idx` ON `LSPE`.`number` (`type_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`identification`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`identification` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`identification` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` TEXT NOT NULL,
  `ident_number_id` INT NOT NULL,
  `birth_place` VARCHAR(45) NOT NULL,
  `birth_date` VARCHAR(45) NOT NULL,
  `gender` VARCHAR(45) NOT NULL,
  `home_address_id` INT NOT NULL,
  `home_phone_id` INT NOT NULL,
  `office_address_id` INT NULL,
  `office_phone_id` INT NULL,
  `email_address_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_identification_address1`
    FOREIGN KEY (`home_address_id`)
    REFERENCES `LSPE`.`address` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_identification_address2`
    FOREIGN KEY (`office_address_id`)
    REFERENCES `LSPE`.`address` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_identification_phone1`
    FOREIGN KEY (`home_phone_id`)
    REFERENCES `LSPE`.`phone` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_identification_phone2`
    FOREIGN KEY (`office_phone_id`)
    REFERENCES `LSPE`.`phone` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_identification_number1`
    FOREIGN KEY (`ident_number_id`)
    REFERENCES `LSPE`.`number` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_identification_address3`
    FOREIGN KEY (`email_address_id`)
    REFERENCES `LSPE`.`address` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_identification_address1_idx` ON `LSPE`.`identification` (`home_address_id` ASC);

CREATE INDEX `fk_identification_address2_idx` ON `LSPE`.`identification` (`office_address_id` ASC);

CREATE INDEX `fk_identification_phone1_idx` ON `LSPE`.`identification` (`home_phone_id` ASC);

CREATE INDEX `fk_identification_phone2_idx` ON `LSPE`.`identification` (`office_phone_id` ASC);

CREATE INDEX `fk_identification_number1_idx` ON `LSPE`.`identification` (`ident_number_id` ASC);

CREATE INDEX `fk_identification_address3_idx` ON `LSPE`.`identification` (`email_address_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`user_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`user_type` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`user_type` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`user`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`user` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`user` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(45) NOT NULL,
  `password` VARCHAR(45) NOT NULL,
  `type_id` INT NOT NULL,
  `identification_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  `status` TINYINT(1) NOT NULL DEFAULT 1,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_user_identification`
    FOREIGN KEY (`identification_id`)
    REFERENCES `LSPE`.`identification` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_user_user_type1`
    FOREIGN KEY (`type_id`)
    REFERENCES `LSPE`.`user_type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_user_identification_idx` ON `LSPE`.`user` (`identification_id` ASC);

CREATE INDEX `fk_user_user_type1_idx` ON `LSPE`.`user` (`type_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`rfid`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`rfid` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`rfid` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `number` VARCHAR(45) NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`picture_type`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`picture_type` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`picture_type` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `LSPE`.`picture`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`picture` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`picture` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `path` TEXT NOT NULL,
  `type_id` INT NOT NULL,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_picture_picture_type1`
    FOREIGN KEY (`type_id`)
    REFERENCES `LSPE`.`picture_type` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_picture_picture_type1_idx` ON `LSPE`.`picture` (`type_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`assessor`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`assessor` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`assessor` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `identification_id` INT NOT NULL,
  `status` INT NOT NULL DEFAULT 1,
  `create_by` INT NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_assesor_identification1`
    FOREIGN KEY (`identification_id`)
    REFERENCES `LSPE`.`identification` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_assesor_identification1_idx` ON `LSPE`.`assessor` (`identification_id` ASC);


-- -----------------------------------------------------
-- Table `LSPE`.`certification`
-- -----------------------------------------------------
DROP TABLE IF EXISTS `LSPE`.`certification` ;

CREATE TABLE IF NOT EXISTS `LSPE`.`certification` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `rfid` INT NOT NULL,
  `photo_picture_id` INT NOT NULL,
  `cert_number_id` INT NOT NULL,
  `certification_picture_id` INT NOT NULL,
  `reg_number_id` INT NOT NULL,
  `kbji_number_id` INT NOT NULL,
  `competence` VARCHAR(45) NOT NULL,
  `identification_id` INT NOT NULL,
  `start_date` VARCHAR(45) NOT NULL,
  `end_date` VARCHAR(45) NOT NULL,
  `create_by` INT NOT NULL,
  `assessor_id` INT NOT NULL,
  `assessment_place_id` INT NOT NULL,
  `assessment_date` VARCHAR(45) NOT NULL,
  `create_date` VARCHAR(45) NOT NULL,
  `update_by` INT NOT NULL,
  `update_date` VARCHAR(45) NOT NULL,
  PRIMARY KEY (`id`),
  CONSTRAINT `fk_certification_rfid1`
    FOREIGN KEY (`rfid`)
    REFERENCES `LSPE`.`rfid` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_number1`
    FOREIGN KEY (`cert_number_id`)
    REFERENCES `LSPE`.`number` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_number2`
    FOREIGN KEY (`reg_number_id`)
    REFERENCES `LSPE`.`number` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_number3`
    FOREIGN KEY (`kbji_number_id`)
    REFERENCES `LSPE`.`number` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_identification1`
    FOREIGN KEY (`identification_id`)
    REFERENCES `LSPE`.`identification` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_picture1`
    FOREIGN KEY (`photo_picture_id`)
    REFERENCES `LSPE`.`picture` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_picture2`
    FOREIGN KEY (`certification_picture_id`)
    REFERENCES `LSPE`.`picture` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_address1`
    FOREIGN KEY (`assessment_place_id`)
    REFERENCES `LSPE`.`address` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION,
  CONSTRAINT `fk_certification_assesor1`
    FOREIGN KEY (`assessor_id`)
    REFERENCES `LSPE`.`assessor` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;

CREATE INDEX `fk_certification_rfid1_idx` ON `LSPE`.`certification` (`rfid` ASC);

CREATE INDEX `fk_certification_number1_idx` ON `LSPE`.`certification` (`cert_number_id` ASC);

CREATE INDEX `fk_certification_number2_idx` ON `LSPE`.`certification` (`reg_number_id` ASC);

CREATE INDEX `fk_certification_number3_idx` ON `LSPE`.`certification` (`kbji_number_id` ASC);

CREATE INDEX `fk_certification_identification1_idx` ON `LSPE`.`certification` (`identification_id` ASC);

CREATE INDEX `fk_certification_picture1_idx` ON `LSPE`.`certification` (`photo_picture_id` ASC);

CREATE INDEX `fk_certification_picture2_idx` ON `LSPE`.`certification` (`certification_picture_id` ASC);

CREATE INDEX `fk_certification_address1_idx` ON `LSPE`.`certification` (`assessment_place_id` ASC);

CREATE INDEX `fk_certification_assesor1_idx` ON `LSPE`.`certification` (`assessor_id` ASC);


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;

-- -----------------------------------------------------
-- Data for table `LSPE`.`address_type`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`address_type` (`id`, `name`) VALUES (DEFAULT, 'home');
INSERT INTO `LSPE`.`address_type` (`id`, `name`) VALUES (DEFAULT, 'office');
INSERT INTO `LSPE`.`address_type` (`id`, `name`) VALUES (DEFAULT, 'email');
INSERT INTO `LSPE`.`address_type` (`id`, `name`) VALUES (DEFAULT, 'assessment place');

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`address`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`address` (`id`, `address`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, 'Rumah Admin', 1, 0, unix_timestamp(), 0, unix_timestamp());
INSERT INTO `LSPE`.`address` (`id`, `address`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, 'Kantor Admin', 2, 0, unix_timestamp(), 0, unix_timestamp());
INSERT INTO `LSPE`.`address` (`id`, `address`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, 'admin@email.com', 3, 0, unix_timestamp(), 0, unix_timestamp());

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`phone_type`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`phone_type` (`id`, `name`) VALUES (DEFAULT, 'home');
INSERT INTO `LSPE`.`phone_type` (`id`, `name`) VALUES (DEFAULT, 'office');

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`phone`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`phone` (`id`, `number`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, '+621234567890', 1, 0, unix_timestamp(), 0, unix_timestamp());
INSERT INTO `LSPE`.`phone` (`id`, `number`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, '+685212345678', 2, 0, unix_timestamp(), 0, unix_timestamp());

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`number_type`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`number_type` (`id`, `name`) VALUES (DEFAULT, 'identification');
INSERT INTO `LSPE`.`number_type` (`id`, `name`) VALUES (DEFAULT, 'registration');
INSERT INTO `LSPE`.`number_type` (`id`, `name`) VALUES (DEFAULT, 'certification');
INSERT INTO `LSPE`.`number_type` (`id`, `name`) VALUES (DEFAULT, 'KBJI');

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`number`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`number` (`id`, `number`, `type_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, '0', 1, 0, unix_timestamp(), 0, unix_timestamp());

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`identification`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`identification` (`id`, `name`, `ident_number_id`, `birth_place`, `birth_date`, `gender`, `home_address_id`, `home_phone_id`, `office_address_id`, `office_phone_id`, `email_address_id`, `create_by`, `create_date`, `update_by`, `update_date`) VALUES (DEFAULT, 'Admin', 1, 'Home', from_unixtime(unix_timestamp(), '%d-%m-%Y'), 'Male', 1, 1, 2, 2, 3, 0, unix_timestamp(), 0, unix_timestamp());

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`user_type`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`user_type` (`id`, `name`) VALUES (DEFAULT, 'superuser');
INSERT INTO `LSPE`.`user_type` (`id`, `name`) VALUES (DEFAULT, 'admin');

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`user`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`user` (`id`, `username`, `password`, `type_id`, `identification_id`, `create_by`, `create_date`, `update_by`, `update_date`, `status`) VALUES (DEFAULT, 'admin', '21232f297a57a5a743894a0e4a801fc3', 1, 1, 0, unix_timestamp(), 0, unix_timestamp(), DEFAULT);

COMMIT;


-- -----------------------------------------------------
-- Data for table `LSPE`.`picture_type`
-- -----------------------------------------------------
START TRANSACTION;
USE `LSPE`;
INSERT INTO `LSPE`.`picture_type` (`id`, `name`) VALUES (DEFAULT, 'photo profile');
INSERT INTO `LSPE`.`picture_type` (`id`, `name`) VALUES (DEFAULT, 'certification');

COMMIT;

