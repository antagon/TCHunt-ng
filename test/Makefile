RUN=bash
SAMPLES_DIR=./samples

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

TESTME_FILES=$(PUBKEYARMOR_FILE) $(PUBKEYRAW_FILE) $(SECKEYARMOR_FILE)\
				$(SECKEYRAW_FILE) $(PLAINTEXT_FILE) $(GPGMSGARMOR_FILE)\
				$(GPGMSGRAW_FILE) $(LUKS_FILE) $(BCRYPT_FILE)\
				$(BCRYPTNCOMPR_FILE)

DELETEME_FILES=$(LUKS_FILE) $(BCRYPT_FILE) $(BCRYPTNCOMPR_FILE)

.PHONY: test generate clean $(addprefix test_, $(TESTME_FILES))

all: generate test

#
# File generators
#
generate: $(LUKS_FILE) $(BCRYPT_FILE) $(BCRYPTNCOMPR_FILE)

$(LUKS_FILE):
	@$(RUN) genfile.sh "Generating '$@' ..." "luks" "$@" "8"

$(BCRYPT_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" 2>/dev/null

$(BCRYPTNCOMPR_FILE):
	@$(RUN) encfile.sh "Generating '$@' ..." "bcrypt" "$(PLAINTEXT_FILE)" "$@" true 2>/dev/null

#
# File testers
#
test: $(addprefix test_, $(TESTME_FILES))

test_$(LUKS_FILE):
	@$(RUN) testfile.sh "Testing '$(LUKS_FILE)' ... " 0 -p "$(LUKS_FILE)"

test_$(PUBKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(PUBKEYARMOR_FILE)' ... " 0 -p "$(PUBKEYARMOR_FILE)"

test_$(PUBKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(PUBKEYRAW_FILE)' ... " 0 -p "$(PUBKEYRAW_FILE)"

test_$(SECKEYARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(SECKEYARMOR_FILE)' ... " 0 -p "$(SECKEYARMOR_FILE)"

test_$(SECKEYRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(SECKEYRAW_FILE)' ... " 0 -p "$(SECKEYRAW_FILE)"

test_$(PLAINTEXT_FILE):
	@$(RUN) testfile.sh "Testing '$(PLAINTEXT_FILE)' ... " 0 -p "$(PLAINTEXT_FILE)"

test_$(GPGMSGARMOR_FILE):
	@$(RUN) testfile.sh "Testing '$(GPGMSGARMOR_FILE)' ... " 0 -p "$(GPGMSGARMOR_FILE)"

test_$(GPGMSGRAW_FILE):
	@$(RUN) testfile.sh "Testing '$(GPGMSGRAW_FILE)' ... " 0 -p "$(GPGMSGRAW_FILE)"

test_$(BCRYPT_FILE):
	@$(RUN) testfile.sh "Testing '$(BCRYPT_FILE)' ... " 0 -p "$(BCRYPT_FILE)"

test_$(BCRYPTNCOMPR_FILE):
	@$(RUN) testfile.sh "Testing '$(BCRYPTNCOMPR_FILE)' ... " 0 -p "$(BCRYPTNCOMPR_FILE)"

clean:
	rm -f $(DELETEME_FILES)

