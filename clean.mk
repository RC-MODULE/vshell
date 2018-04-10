ifndef OS_RM
 	OS_RM    =rm -f -r
  	OS_RD    =rm -d -r
endif

kill: clean
	-$(OS_RM) *.vcproj *.sln *.vcxproj *.db output output.264 

clean:
	-$(OS_RM) *.abs *.elf *.o *.ncb *.map *.elf *.suo *.user *.filters *.db .* *.opendb $(TARGET) nmc4vars_win.mk $(CLEAN_FILES)
	-$(OS_RD) Debug Release bin obj .vs $(CLEAN_DIRS)
