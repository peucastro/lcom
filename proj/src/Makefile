# name of the program (Minix service)
PROG=proj

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic

# list of library dependencies (LCF)
DPADD += ${LIBLCF}
LDADD += -llcf

# define headers dependencies
HDRS = utils/utils.h \
       controller/timer/i8254.h controller/timer/timer.h \
       controller/kbc/i8042.h controller/kbc/kbc.h controller/kbc/kbd.h controller/kbc/mouse.h \
       controller/graphics/vbe.h controller/graphics/graphics.h

# compilation rules for each source file
proj.o: proj.c ${HDRS}
	${CC} ${CFLAGS} -c $< -o $@

utils/utils.o: utils/utils.c utils/utils.h
	${CC} ${CFLAGS} -c $< -o $@

controller/timer/timer.o: controller/timer/timer.c controller/timer/timer.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/kbc.o: controller/kbc/kbc.c controller/kbc/kbc.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/kbd.o: controller/kbc/kbd.c controller/kbc/kbd.h
	${CC} ${CFLAGS} -c $< -o $@

controller/kbc/mouse.o: controller/kbc/mouse.c controller/kbc/mouse.h
	${CC} ${CFLAGS} -c $< -o $@

controller/graphics/vbe.o: controller/graphics/vbe.c controller/graphics/vbe.h
	${CC} ${CFLAGS} -c $< -o $@

controller/graphics/graphics.o: controller/graphics/graphics.c controller/graphics/graphics.h
	${CC} ${CFLAGS} -c $< -o $@

controller/ih/ih.o: controller/ih/ih.c controller/ih/ih.h
	${CC} ${CFLAGS} -c $< -o $@

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
