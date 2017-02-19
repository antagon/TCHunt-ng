#
# TCHunt-ng - reveal (possibly) encrypted files stored on a filesystem.
# Copyright (C) 2016, 2017  CodeWard.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
export TCHUNTNG_BIN=../src/tchuntng
export TCHUNTNG_FLAGS=-p
export TEST_PEDANTIC=no

RUN=/bin/bash
SAMPLES_DIR=./samples

#
# Sample files
#
PUBKEYARMOR_FILE=$(SAMPLES_DIR)/gpg_pubkey.asc
PUBKEYRAW_FILE=$(SAMPLES_DIR)/gpg_pubkey.dat
SECKEYARMOR_FILE=$(SAMPLES_DIR)/gpg_secretkey.asc
SECKEYRAW_FILE=$(SAMPLES_DIR)/gpg_secretkey.dat
PLAINTEXT_FILE=$(SAMPLES_DIR)/message.txt
NEXIST_FILE=$(SAMPLES_DIR)/idonotexist.txt
GPGMSGARMOR_FILE=$(SAMPLES_DIR)/message.txt.asc
GPGMSGRAW_FILE=$(SAMPLES_DIR)/message.txt.gpg
LUKS_FILE=$(SAMPLES_DIR)/luks_container.dat
VCCONTAINER_FILE=$(SAMPLES_DIR)/vera_container.hc
BCRYPT_GZ_FILE=$(PLAINTEXT_FILE).gz.bcrypt
BCRYPT_FILE=$(PLAINTEXT_FILE).bcrypt
OPENSSL_ARMORED_AES256_FILE=$(SAMPLES_DIR)/message.txt.aes256.armored.openssl
OPENSSL_AES256_FILE=$(SAMPLES_DIR)/message.txt.aes256.openssl
CCRYPT_FILE=$(PLAINTEXT_FILE).ccrypt
ENCFS_FILES=$(SAMPLES_DIR)/karabina_encfs
MCRYPT_BARE_AES256_GZ_FILE=$(PLAINTEXT_FILE).gz.aes256.bare.mcrypt
MCRYPT_AES256_GZ_FILE=$(PLAINTEXT_FILE).gz.aes256.mcrypt
MCRYPT_BARE_TWOFISH_GZ_FILE=$(PLAINTEXT_FILE).gz.twofish.bare.mcrypt
MCRYPT_TWOFISH_GZ_FILE=$(PLAINTEXT_FILE).gz.twofish.mcrypt
MCRYPT_BARE_ARCFOUR_GZ_FILE=$(PLAINTEXT_FILE).gz.arcfour.bare.mcrypt
MCRYPT_ARCFOUR_GZ_FILE=$(PLAINTEXT_FILE).gz.arcfour.mcrypt
MCRYPT_BARE_BLOWFISH_GZ_FILE=$(PLAINTEXT_FILE).gz.blowfish.bare.mcrypt
MCRYPT_BLOWFISH_GZ_FILE=$(PLAINTEXT_FILE).gz.blowfish.mcrypt


DEPENDS_ON=$(TCHUNTNG_BIN) bcrypt openssl ccrypt mcrypt dd veracrypt\
				fallocate cryptsetup

GENERATEME_FILES=$(LUKS_FILE) $(BCRYPT_GZ_FILE) $(BCRYPT_FILE)\
					$(VCCONTAINER_FILE) $(OPENSSL_ARMORED_AES256_FILE)\
					$(OPENSSL_AES256_FILE) $(CCRYPT_FILE) $(MCRYPT_BARE_AES256_GZ_FILE)\
					$(MCRYPT_AES256_GZ_FILE) $(MCRYPT_BARE_TWOFISH_GZ_FILE)\
					$(MCRYPT_TWOFISH_GZ_FILE) $(MCRYPT_BARE_ARCFOUR_GZ_FILE)\
					$(MCRYPT_ARCFOUR_GZ_FILE) $(MCRYPT_BARE_BLOWFISH_GZ_FILE)\
					$(MCRYPT_BLOWFISH_GZ_FILE)

TESTME_FILES=$(PUBKEYARMOR_FILE) $(PUBKEYRAW_FILE) $(SECKEYARMOR_FILE)\
				$(SECKEYRAW_FILE) $(PLAINTEXT_FILE) $(GPGMSGARMOR_FILE)\
				$(GPGMSGRAW_FILE) $(LUKS_FILE) $(BCRYPT_GZ_FILE)\
				$(BCRYPT_FILE) $(VCCONTAINER_FILE) $(OPENSSL_ARMORED_AES256_FILE)\
				$(OPENSSL_AES256_FILE) $(CCRYPT_FILE) $(ENCFS_FILES) $(NEXIST_FILE)\
				$(CCRYPT_FILE) $(MCRYPT_BARE_AES256_GZ_FILE) $(MCRYPT_AES256_GZ_FILE)\
				$(MCRYPT_BARE_TWOFISH_GZ_FILE) $(MCRYPT_TWOFISH_GZ_FILE)\
				$(MCRYPT_BARE_ARCFOUR_GZ_FILE) $(MCRYPT_ARCFOUR_GZ_FILE)\
				$(MCRYPT_BARE_BLOWFISH_GZ_FILE) $(MCRYPT_BLOWFISH_GZ_FILE)

DELETEME_FILES=$(LUKS_FILE) $(BCRYPT_GZ_FILE) $(BCRYPT_FILE)\
				$(VCCONTAINER_FILE) $(OPENSSL_ARMORED_AES256_FILE) $(OPENSSL_AES256_FILE)\
				$(CCRYPT_FILE) $(MCRYPT_BARE_AES256_GZ_FILE) $(MCRYPT_AES256_GZ_FILE)\
				$(MCRYPT_BARE_TWOFISH_GZ_FILE) $(MCRYPT_TWOFISH_GZ_FILE) $(MCRYPT_BARE_ARCFOUR_GZ_FILE)\
				$(MCRYPT_ARCFOUR_GZ_FILE) $(MCRYPT_BARE_BLOWFISH_GZ_FILE) $(MCRYPT_BLOWFISH_GZ_FILE)

.PHONY: all clean deps prereq $(DEPENDS_ON) generate test test_init $(addprefix test_, $(TESTME_FILES))

all: prereq generate test

#
# Check prerequisites
#
prereq: $(DEPENDS_ON)
	@$(RUN) checkdep.sh "Checking the dependency list ..." $(DEPENDS_ON)

#
# File generators
#
generate: $(GENERATEME_FILES)

$(LUKS_FILE):
	@$(RUN) genfile.sh "Generating '$@' ..." "luks" "$@" "4"

$(BCRYPT_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" 2>/dev/null

$(BCRYPT_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" true 2>/dev/null

$(VCCONTAINER_FILE):
	@$(RUN) genfile.sh "Generating '$@' ..." "veracrypt" "$@" "4"

$(OPENSSL_ARMORED_AES256_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "openssl" "$(PLAINTEXT_FILE)" "$@" "aes-256-cbc" true

$(OPENSSL_AES256_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "openssl" "$(PLAINTEXT_FILE)" "$@" "aes-256-cbc"

$(CCRYPT_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "ccrypt" "$(PLAINTEXT_FILE)" "$@"

$(MCRYPT_BARE_AES256_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "rijndael-256" "cbc" true 2>/dev/null

$(MCRYPT_AES256_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "rijndael-256" "cbc" 2>/dev/null

$(MCRYPT_BARE_TWOFISH_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "twofish" "cbc" true 2>/dev/null

$(MCRYPT_TWOFISH_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "twofish" "cbc" 2>/dev/null

$(MCRYPT_BARE_ARCFOUR_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "arcfour" "cbc" true 2>/dev/null

$(MCRYPT_ARCFOUR_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "arcfour" "cbc" 2>/dev/null

$(MCRYPT_BARE_BLOWFISH_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "blowfish" "cbc" true 2>/dev/null

$(MCRYPT_BLOWFISH_GZ_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "mcrypt" "$(PLAINTEXT_FILE)" "$@" "blowfish" "cbc" 2>/dev/null

#
# File testers
#
test: test_init $(addprefix test_, $(TESTME_FILES))

test_init:
	@echo "Using `$(TCHUNTNG_BIN) -v` ($(TCHUNTNG_BIN))"
	@echo "Pedantic mode: $(TEST_PEDANTIC)"

test_$(LUKS_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(PUBKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(PUBKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(SECKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(SECKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(PLAINTEXT_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 2 "$(subst test_,,$@)"
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' (treat no result as success) ..." 0 -q "$(subst test_,,$@)"

test_$(GPGMSGARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(GPGMSGRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(BCRYPT_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(BCRYPT_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(VCCONTAINER_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' (TCHunt compatibility mode) ..." 0 -T "$(subst test_,,$@)"

test_$(OPENSSL_ARMORED_AES256_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(OPENSSL_AES256_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(CCRYPT_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(ENCFS_FILES):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 $(ENCFS_FILES)/*

test_$(NEXIST_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 1 "$(subst test_,,$@)"

test_$(MCRYPT_BARE_AES256_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_AES256_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_BARE_TWOFISH_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_TWOFISH_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_BARE_ARCFOUR_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_ARCFOUR_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_BARE_BLOWFISH_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

test_$(MCRYPT_BLOWFISH_GZ_FILE):
	@$(RUN) testfile.sh "Testing '$(subst test_,,$@)' ..." 0 "$(subst test_,,$@)"

clean:
	@$(RM) -f $(DELETEME_FILES)

deps:
	@which $(DEPENDS_ON)

