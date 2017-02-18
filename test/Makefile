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
export TEST_PEDANTIC=no

RUN=/bin/bash
SAMPLES_DIR=./samples
TCHUNTNG_FLAGS=-p

#
# Sample files
#
PUBKEYARMOR_FILE=$(SAMPLES_DIR)/gpg_pubkey.asc
PUBKEYRAW_FILE=$(SAMPLES_DIR)/gpg_pubkey.dat
SECKEYARMOR_FILE=$(SAMPLES_DIR)/gpg_secretkey.asc
SECKEYRAW_FILE=$(SAMPLES_DIR)/gpg_secretkey.dat
PLAINTEXT_FILE=$(SAMPLES_DIR)/message.txt
GPGMSGARMOR_FILE=$(SAMPLES_DIR)/message.txt.asc
GPGMSGRAW_FILE=$(SAMPLES_DIR)/message.txt.gpg
LUKS_FILE=$(SAMPLES_DIR)/luks_container.dat
BCRYPT_FILE=$(PLAINTEXT_FILE).bfe
BCRYPTNCOMPR_FILE=$(PLAINTEXT_FILE)-nocompr.bfe
VCCONTAINER_FILE=$(SAMPLES_DIR)/vera_container.hc
AESENCARMOR_FILE=$(SAMPLES_DIR)/message.txt.aes-armored.enc
AESENCRAW_FILE=$(SAMPLES_DIR)/message.txt.aes.enc
CCRYPT_FILE=$(PLAINTEXT_FILE).cpt
ENCFS_FILES=$(SAMPLES_DIR)/karabina_encfs
NEXIST_FILE=$(SAMPLES_DIR)/idonotexist.txt

DEPENDENCY_LIST=$(TCHUNTNG_BIN) bcrypt openssl ccrypt dd veracrypt\
				fallocate cryptsetup

GENERATEME_FILES=$(LUKS_FILE) $(BCRYPT_FILE) $(BCRYPTNCOMPR_FILE)\
					$(VCCONTAINER_FILE) $(AESENCARMOR_FILE) $(AESENCRAW_FILE)\
					$(CCRYPT_FILE)

TESTME_FILES=$(PUBKEYARMOR_FILE) $(PUBKEYRAW_FILE) $(SECKEYARMOR_FILE)\
				$(SECKEYRAW_FILE) $(PLAINTEXT_FILE) $(GPGMSGARMOR_FILE)\
				$(GPGMSGRAW_FILE) $(LUKS_FILE) $(BCRYPT_FILE)\
				$(BCRYPTNCOMPR_FILE) $(VCCONTAINER_FILE) $(AESENCARMOR_FILE)\
				$(AESENCRAW_FILE) $(CCRYPT_FILE) $(ENCFS_FILES) $(NEXIST_FILE)

DELETEME_FILES=$(LUKS_FILE) $(BCRYPT_FILE) $(BCRYPTNCOMPR_FILE)\
				$(VCCONTAINER_FILE) $(AESENCARMOR_FILE) $(AESENCRAW_FILE)\
				$(CCRYPT_FILE)

.PHONY: all prereq $(DEPENDENCY_LIST) generate test test_init $(addprefix test_, $(TESTME_FILES)) clean

all: prereq generate test

#
# Check prerequisites
#
prereq: $(DEPENDENCY_LIST)
	@$(RUN) checkdep.sh "Checking the dependency list ..." $(DEPENDENCY_LIST)

#
# File generators
#
generate: $(GENERATEME_FILES)

$(LUKS_FILE):
	@$(RUN) genfile.sh "Generating '$@' ..." "luks" "$@" "4"

$(BCRYPT_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" 2>/dev/null

$(BCRYPTNCOMPR_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" true 2>/dev/null

$(VCCONTAINER_FILE):
	@$(RUN) genfile.sh "Generating '$@' ..." "veracrypt" "$@" "4"

$(AESENCARMOR_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "openssl" "$(PLAINTEXT_FILE)" "$@" "aes-256-cbc" true

$(AESENCRAW_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "openssl" "$(PLAINTEXT_FILE)" "$@" "aes-256-cbc"

$(CCRYPT_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "ccrypt" "$(PLAINTEXT_FILE)" "$@"

#
# File testers
#
test: test_init $(addprefix test_, $(TESTME_FILES))

test_init:
	@echo "Using `$(TCHUNTNG_BIN) -v` ($(TCHUNTNG_BIN))"
	@echo "Pedantic mode: $(TEST_PEDANTIC)"

test_$(LUKS_FILE):
	@$(RUN) testfile.sh "Testing '$(LUKS_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(LUKS_FILE)"

test_$(PUBKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(PUBKEYARMOR_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(PUBKEYARMOR_FILE)"

test_$(PUBKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(PUBKEYRAW_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(PUBKEYRAW_FILE)"

test_$(SECKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(SECKEYARMOR_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(SECKEYARMOR_FILE)"

test_$(SECKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(SECKEYRAW_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(SECKEYRAW_FILE)"

test_$(PLAINTEXT_FILE):
	@$(RUN) testfile.sh "Testing '$(PLAINTEXT_FILE)' ..." 2 $(TCHUNTNG_FLAGS) "$(PLAINTEXT_FILE)"
	@$(RUN) testfile.sh "Testing '$(PLAINTEXT_FILE)' (treat no result as success) ..." 0 $(TCHUNTNG_FLAGS) -q "$(PLAINTEXT_FILE)"

test_$(GPGMSGARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(GPGMSGARMOR_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(GPGMSGARMOR_FILE)"

test_$(GPGMSGRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(GPGMSGRAW_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(GPGMSGRAW_FILE)"

test_$(BCRYPT_FILE):
	@$(RUN) testfile.sh "Testing '$(BCRYPT_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(BCRYPT_FILE)"

test_$(BCRYPTNCOMPR_FILE):
	@$(RUN) testfile.sh "Testing '$(BCRYPTNCOMPR_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(BCRYPTNCOMPR_FILE)"

test_$(VCCONTAINER_FILE):
	@$(RUN) testfile.sh "Testing '$(VCCONTAINER_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(VCCONTAINER_FILE)"
	@$(RUN) testfile.sh "Testing '$(VCCONTAINER_FILE)' (TCHunt compatibility mode) ..." 0 $(TCHUNTNG_FLAGS) -T "$(VCCONTAINER_FILE)"

test_$(AESENCARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(AESENCARMOR_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(AESENCARMOR_FILE)"

test_$(AESENCRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(AESENCRAW_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(AESENCRAW_FILE)"

test_$(CCRYPT_FILE):
	@$(RUN) testfile.sh "Testing '$(CCRYPT_FILE)' ..." 0 $(TCHUNTNG_FLAGS) "$(CCRYPT_FILE)"

test_$(ENCFS_FILES):
	@$(RUN) testfile.sh "Testing '$(ENCFS_FILES)' ..." 0 $(TCHUNTNG_FLAGS) $(ENCFS_FILES)/*

test_$(NEXIST_FILE):
	@$(RUN) testfile.sh "Testing '$(NEXIST_FILE)' ..." 1 $(TCHUNTNG_FLAGS) "$(NEXIST_FILE)"

clean:
	@$(RM) -f $(DELETEME_FILES)

