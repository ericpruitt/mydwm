# URL of the dwm Git repository.
DWM_REPOSITORY = http://git.suckless.org/dwm

# Git revision to be compiled. This can be set to an empty string to compile at
# HEAD.
PINNED_REVISION = 56a31dc

all: dwm

dwm-src:
	@if ! grep -q "^/$@" .git/info/exclude; then \
		echo "/$@" >> .git/info/exclude || exit 1; \
		echo "Added /$@ to local Git exclusion rules."; \
	fi
	git clone $(DWM_REPOSITORY) $@
	@if [ -n "$(strip $(PINNED_REVISION))" ]; then \
		cd dwm-src && git reset --hard $(PINNED_REVISION); \
	fi

apply:
	@echo "Applying patches:"; \
	for patch in patches/*.diff; do \
		echo "- $$patch"; \
		if ! patch -d dwm-src -s < $$patch; then \
			exit 1; \
		fi; \
	done

# The if/wildcard combination ensures that the dwm-src directory is only
# considered a dependency if it does not already exist. This ensures changing
# directory timestamps don't result in unnecessary recompiles.
dwm-src/config.h: config.h $(if $(wildcard dwm-src),,dwm-src)
	cp config.h $@

# The wildcard expansion is used to only include *.c files inside the dwm-src
# folder if the folder exists when Makefile is parsed. Using dwm-src/*.c alone
# would cause the recipe to fail when the *.c files did not exist because
# there's no recipe for them in this Makefile.
dwm-src/dwm: dwm-src/config.h build.mk patches/* $(wildcard dwm-src/*.c)
	@if (cd dwm-src && git diff --quiet -- *.c); then \
		$(MAKE) -s apply || exit; \
	else \
		if echo $? | grep -q patches/; then \
			echo "The patch set has been modified, but the" \
			"changes cannot be applied because the dwm tree is" \
			"dirty."; \
			exit 1; \
		fi; \
		echo "WARNING: tree is dirty; no patches will be applied."; \
	fi
	cp build.mk dwm-src/config.mk
	cd dwm-src && $(MAKE)

dwm: dwm-src/dwm
	cp $^ $@

sanity:
	@set -e; \
	$(MAKE) clean > /dev/null; \
	for patch in patches/00-*.diff; do \
		if ! patch -s -d dwm-src < $$patch; then \
			echo "$$patch failed."; \
			exit 1; \
		fi; \
		$(MAKE) clean > /dev/null; \
	done; \
	echo "OK"

clean:
	@set -e; \
	if [ -e dwm-src ]; then \
		cd dwm-src; \
		$(MAKE) -s clean; \
		git reset --hard $(PINNED_REVISION); \
	fi
	rm -f dwm

cleaner:
	rm -rf dwm-src dwm

update: clean
	@cd dwm-src && git pull

.PHONY: all apply clean cleaner sanity update
