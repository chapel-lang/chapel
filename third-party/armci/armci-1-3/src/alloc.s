	.set noat
	.set noreorder
	.data
	.lcomm	$$1$info 32
	.lcomm	$$2$offsets 512
	.data
	.lcomm	$$3$armci_base_map 8
	.data
$$1:
$$2:
	.ascii	"smp count too large\X00"

	.byte	0 : 4
$$3:
	.ascii	"smp count bad\X00"

	.byte	0 : 2
$$4:
	.ascii	"getaddressconf failed\X00"

	.byte	0 : 2
$$5:
	.ascii	"******armci_base_map = %p \X0A\X00"

	.byte	0 : 4
$$6:
	.ascii	"smp count too large\X00"

	.byte	0 : 4
$$7:
	.ascii	">shmem_reserve failed\X00"

	.byte	0 : 2
$$8:
	.ascii	"%d i=%d before =%p %p reserved=%ld size=%ld bytes=%ld pid=%ld\X0A\X00"

	.byte	0 : 1
$$9:
	.ascii	"failed:end=\X00"

	.byte	0 : 4
$$10:
	.ascii	"%d: fixup p=%d peer=%d %p ->>> %p\X0A\X00"

	.byte	0 : 5
	.byte	0 : 8
	.sdata
$$11:
$$4$armci_reserve_bytes:
	.quad	0x0 # .quad 0
	.byte	0 : 8
	.rdata
$$12:
	.extern armci_clus_first 4
	.extern armci_clus_last 4
	.extern armci_me 4
	.extern armci_master 4
	.text
	.arch	generic
	.align 4
	.file 1 "alloc.c"
	.loc 1 34
	.globl  armci_init_alloc
	.ent 	armci_init_alloc
	.loc 1 34
armci_init_alloc:
	.context full
	ldah	$gp, ($27)!gpdisp!1
	unop
	lda	$gp, ($gp)!gpdisp!1
	unop
L$29:
	lda	$sp, -256($sp)
	.loc 1 40
	ldq	$27, getpagesize($gp)!literal!2										   # 000040
	.loc 1 34
	stq	$26, ($sp)												   # 000034
	stq	$9, 8($sp)
	.mask 0x04000200,-256
	.fmask 0x00000000,0
	.frame  $sp, 256, $26
	.prologue 1
	.loc 1 35
	sextl	$17, $9													   # 000035
	.loc 1 34
	stq	$16, 16($sp)												   # 000034
	.loc 1 40
	jsr	$26, ($27), getpagesize!lituse_jsr!2									   # 000040
	ldah	$gp, ($26)!gpdisp!3
	.loc 1 42
	cmple	$9, 64, $1												   # 000042
	.loc 1 40
	stq	$0, 24($sp)												   # 000040
	.loc 1 42
	mov	$9, $17													   # 000042
	.loc 1 40
	lda	$gp, ($gp)!gpdisp!3											   # 000040
	.loc 1 42
	bne	$1, L$30												   # 000042
	ldq	$27, ARMCI_Error($gp)!literal!4
	ldq	$16, $$2+96($gp)!literal!5
	lda	$16, -96($16)!lituse_base!5
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!4
	ldah	$gp, ($26)!gpdisp!6
	lda	$gp, ($gp)!gpdisp!6
L$30:
	.loc 1 43
	mov	$9, $17													   # 000043
	bgt	$9, L$35
	ldq	$27, ARMCI_Error($gp)!literal!7
	ldq	$16, $$3+72($gp)!literal!8
	lda	$16, -72($16)!lituse_base!8
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!7
	ldah	$gp, ($26)!gpdisp!9
	lda	$gp, ($gp)!gpdisp!9
L$35:
	.loc 1 45
	lda	$16, 32($sp)												   # 000045
	mov	224, $17
	ldq	$27, getaddressconf($gp)!literal!10
	jsr	$26, ($27), getaddressconf!lituse_jsr!10
	ldah	$gp, ($26)!gpdisp!11
	mov	$0, $17
	lda	$gp, ($gp)!gpdisp!11
	.loc 1 46
	bge	$0, L$37												   # 000046
	ldq	$27, ARMCI_Error($gp)!literal!12
	ldq	$16, $$4+56($gp)!literal!13
	lda	$16, -56($16)!lituse_base!13
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!12
	ldah	$gp, ($26)!gpdisp!14
	lda	$gp, ($gp)!gpdisp!14
L$37:
	.loc 1 48
	ldq	$0, 208($sp)												   # 000048
	.loc 1 52
	mov	512, $17												   # 000052
	ldq	$27, bzero($gp)!literal!15
	ldq	$1, $$1$info+32($gp)!literal!16
	.loc 1 37
	ldq	$2, $$1$info+32($gp)!literal!17										   # 000037
	.loc 1 52
	lda	$1, -32($1)!lituse_base!16										   # 000052
	.loc 1 37
	lda	$2, -32($2)!lituse_base!17										   # 000037
	.loc 1 52
	lda	$16, 32($1)												   # 000052
	.loc 1 48
	stq	$0, ($2)												   # 000048
	.loc 1 52
	jsr	$26, ($27), bzero!lituse_jsr!15										   # 000052
	ldah	$gp, ($26)!gpdisp!18
	.loc 1 55
	ldq	$16, 16($sp)												   # 000055
	ldq	$17, 24($sp)
	mov	1, $2
	.loc 1 52
	lda	$gp, ($gp)!gpdisp!18											   # 000052
	.loc 1 55
	sll	$2, 31, $2												   # 000055
	ldq	$27, _OtsRemainder64Unsigned($gp)!literal!19
	jsr	$26, ($27), _OtsRemainder64Unsigned!lituse_jsr!19
	ldah	$gp, ($26)!gpdisp!20
	ldq	$16, 16($sp)
	.loc 1 56
	mov	$9, $17													   # 000056
	.loc 1 55
	lda	$gp, ($gp)!gpdisp!20											   # 000055
	.loc 1 56
	ldq	$27, _OtsDivide64Unsigned($gp)!literal!21								   # 000056
	.loc 1 55
	cmovne	$0, $2, $16												   # 000055
	.loc 1 56
	jsr	$26, ($27), _OtsDivide64Unsigned!lituse_jsr!21								   # 000056
	ldah	$gp, ($26)!gpdisp!22
	.loc 1 68
	ldq	$26, ($sp)												   # 000068
	.loc 1 59
	srl	$0, 22, $0												   # 000059
	.loc 1 68
	ldq	$9, 8($sp)												   # 000068
	.loc 1 56
	lda	$gp, ($gp)!gpdisp!22											   # 000056
	.loc 1 60
	sll	$0, 22, $0												   # 000060
	.loc 1 37
	ldq	$3, $$1$info+32($gp)!literal!23										   # 000037
	lda	$3, -32($3)!lituse_base!23
	.loc 1 60
	stq	$0, 16($3)												   # 000060
	.loc 1 68
	lda	$sp, 256($sp)												   # 000068
	ret	($26)
	.end 	armci_init_alloc
	unop
	unop
	unop
	.loc 1 34
	.loc 1 70
	.globl  armci_region_getcore
	.ent 	armci_region_getcore
	.loc 1 70
armci_region_getcore:
	.context full
	ldah	$gp, ($27)!gpdisp!24
	unop
	lda	$gp, ($gp)!gpdisp!24
	unop
L$39:
	lda	$sp, -16($sp)
	.loc 1 73
	mov	1, $7													   # 000073
	.loc 1 74
	ldq	$6, armci_clus_first($gp)!literal!25									   # 000074
	ldq	$5, armci_clus_last($gp)!literal!26
	.loc 1 72
	ldq	$3, $$4$armci_reserve_bytes($gp)!literal!27								   # 000072
	.loc 1 74
	ldq	$27, armci_init_alloc($gp)!literal!28									   # 000074
	.loc 1 72
	ldq	$4, ($3)!lituse_base!27											   # 000072
	.loc 1 70
	stq	$26, ($sp)												   # 000070
	.mask 0x04000000,-16
	.fmask 0x00000000,0
	.frame  $sp, 16, $26
	.prologue 1
	stq	$16, 8($sp)
	.loc 1 74
	mov	-1, $16													   # 000074
	ldl	$5, ($5)!lituse_base!26
	ldl	$6, ($6)!lituse_base!25
	.loc 1 72
	bne	$4, L$40												   # 000072
	.loc 1 73
	stq	$7, ($3)!lituse_base!27											   # 000073
	.loc 1 74
	subl	$5, $6, $5												   # 000074
	addl	$5, 1, $17
	jsr	$26, ($27), armci_init_alloc!lituse_jsr!28
	ldah	$gp, ($26)!gpdisp!29
	lda	$gp, ($gp)!gpdisp!29
	.loc 1 75
L$40:															   # 000075
	.loc 1 76
	ldq	$16, 8($sp)												   # 000076
	ldq	$27, malloc($gp)!literal!30
	jsr	$26, ($27), malloc!lituse_jsr!30
	ldah	$gp, ($26)!gpdisp!31
	.loc 1 77
	ldq	$26, ($sp)												   # 000077
	lda	$sp, 16($sp)
	.loc 1 76
	.context none
	lda	$gp, ($gp)!gpdisp!31											   # 000076
	.loc 1 77
	ret	($26)													   # 000077
	.end 	armci_region_getcore
	unop
	.loc 1 70
	.loc 1 80
	.globl  exchange_info
	.ent 	exchange_info
	.loc 1 80
exchange_info:
	.context full
	ldah	$gp, ($27)!gpdisp!32
	unop
	lda	$gp, ($gp)!gpdisp!32
	unop
L$20:
	.loc 1 81
	sextl	$16, $21												   # 000081
	.loc 1 83
	ble	$21, L$42												   # 000083
	.loc 1 80
	lda	$sp, -48($sp)												   # 000080
	stq	$26, ($sp)
	stq	$9, 8($sp)
	stq	$10, 16($sp)
	stq	$11, 24($sp)
	stq	$12, 32($sp)
	stq	$13, 40($sp)
	.mask 0x04003E00,-48
	.fmask 0x00000000,0
	.frame  $sp, 48, $26
	.prologue 1
	clr	$11
	sextl	$16, $10
	mov	$17, $9
	.loc 1 84
	ldq	$13, armci_me($gp)!literal!33										   # 000084
	ldq	$12, armci_master($gp)!literal!34
	unop
	unop
	.loc 1 83
L$25:															   # 000083
	.loc 1 84
	ldl	$3, ($13)												   # 000084
	ldl	$4, ($12)
	.loc 1 86
	mov	1, $18													   # 000086
	mov	$11, $19
	ldq	$27, shmem_put($gp)!literal!35
	.loc 1 84
	subl	$3, $4, $5												   # 000084
	.loc 1 86
	s8addq	$4, $31, $4												   # 000086
	s8addq	$3, $9, $3
	.loc 1 84
	xor	$11, $5, $5												   # 000084
	.loc 1 86
	subq	$3, $4, $17												   # 000086
	subq	$3, $4, $16
	unop
	.loc 1 84
	beq	$5, L$27												   # 000084
	.loc 1 86
	jsr	$26, ($27), shmem_put!lituse_jsr!35									   # 000086
	ldah	$gp, ($26)!gpdisp!36
	lda	$gp, ($gp)!gpdisp!36
	.loc 1 87
L$27:															   # 000087
	.loc 1 83
	addl	$11, 1, $11												   # 000083
	cmplt	$11, $10, $0
	bne	$0, L$25
	.loc 1 88
	ldq	$26, ($sp)												   # 000088
	ldq	$9, 8($sp)
	ldq	$10, 16($sp)
	ldq	$11, 24($sp)
	ldq	$12, 32($sp)
	ldq	$13, 40($sp)
	lda	$sp, 48($sp)
L$42:
	.context none
	ret	($26)
	.end 	exchange_info
	unop
	.loc 1 80
	.loc 1 91
	.globl  armci_region_register
	.ent 	armci_region_register
	.loc 1 91
armci_region_register:
	.context full
	ldah	$gp, ($27)!gpdisp!37
	unop
	lda	$gp, ($gp)!gpdisp!37
	unop
L$7:
	.loc 1 99
	ldq	$3, ($17)												   # 000099
	.loc 1 91
	mov	$17, $22												   # 000091
	.loc 1 92
	sextl	$16, $17												   # 000092
	.loc 1 105
	ldq	$27, ARMCI_Error($gp)!literal!38									   # 000105
	.loc 1 99
	beq	$3, L$44												   # 000099
	.loc 1 105
	cmple	$17, 64, $4												   # 000105
	.loc 1 91
	lda	$sp, -80($sp)												   # 000091
	stq	$26, ($sp)
	stq	$9, 8($sp)
	stq	$10, 16($sp)
	stq	$11, 24($sp)
	stq	$12, 32($sp)
	.mask 0x04001E00,-80
	.fmask 0x00000000,0
	.frame  $sp, 80, $26
	.prologue 1
	mov	$22, $9
	stq	$18, 48($sp)
	sextl	$16, $10
	.loc 1 105
	ldq	$16, $$6($gp)!literal!39										   # 000105
	.loc 1 91
	stq	$19, 56($sp)												   # 000091
	.loc 1 105
	bne	$4, L$11												   # 000105
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!38
	ldah	$gp, ($26)!gpdisp!40
	lda	$gp, ($gp)!gpdisp!40
L$11:
	.loc 1 109
	ldq	$2, 56($sp)												   # 000109
	.loc 1 106
	ldq	$11, $$1$info+32($gp)!literal!41									   # 000106
	.loc 1 109
	ldq	$27, getpagesize($gp)!literal!42									   # 000109
	.loc 1 106
	s8addq	$10, $11, $10												   # 000106
	ldq	$0, ($10)
	bne	$0, L$13
	.loc 1 109
	ldq	$1, ($9)												   # 000109
	mov	5, $0
	addq	$1, $2, $1
	sll	$0, 30, $0
	subq	$1, $0, $1
	stq	$1, 64($sp)
	jsr	$26, ($27), getpagesize!lituse_jsr!42
	ldq	$1, 64($sp)
	ldah	$gp, ($26)!gpdisp!43
	.loc 1 116
	ldq	$16, -32($11)												   # 000116
	clr	$18
	.loc 1 109
	lda	$gp, 4($gp)!gpdisp!43											   # 000109
	.loc 1 116
	ldq	$27, shmem_reserve($gp)!literal!44									   # 000116
	.loc 1 109
	addq	$1, $0, $0												   # 000109
	.loc 1 112
	ldah	$2, 64($0)												   # 000112
	.loc 1 109
	stq	$0, 72($sp)												   # 000109
	.loc 1 112
	lda	$2, -1($2)												   # 000112
	.loc 1 113
	srl	$2, 22, $2												   # 000113
	.loc 1 114
	sll	$2, 22, $12												   # 000114
	.loc 1 116
	mov	$12, $17												   # 000116
	unop
	jsr	$26, ($27), shmem_reserve!lituse_jsr!44
	ldah	$gp, ($26)!gpdisp!45
	stq	$0, -24($11)
	.loc 1 117
	sextl	$12, $17												   # 000117
	.loc 1 116
	lda	$gp, ($gp)!gpdisp!45											   # 000116
	.loc 1 117
	bne	$0, L$15												   # 000117
	ldq	$27, ARMCI_Error($gp)!literal!46
	ldq	$16, $$7-24($gp)!literal!47
	lda	$16, 24($16)!lituse_base!47
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!46
	ldah	$gp, ($26)!gpdisp!48
	lda	$gp, ($gp)!gpdisp!48
	.loc 1 120
L$15:															   # 000120
	.loc 1 122
	mov	5, $17													   # 000122
	ldl	$20, 48($sp)
	ldq	$16, -24($11)
	sll	$17, 30, $17
	mov	$12, $18
	ldq	$19, 72($sp)
	ldq	$27, shmem_map($gp)!literal!49
	lda	$21, 40($sp)
	jsr	$26, ($27), shmem_map!lituse_jsr!49
	ldah	$gp, ($26)!gpdisp!50
	.loc 1 123
	clr	$17													   # 000123
	.loc 1 122
	lda	$gp, ($gp)!gpdisp!50											   # 000122
	.loc 1 123
	bne	$0, L$17												   # 000123
	ldq	$27, ARMCI_Error($gp)!literal!51
	ldq	$16, $$9-112($gp)!literal!52
	lda	$16, 112($16)!lituse_base!52
	jsr	$26, ($27), ARMCI_Error!lituse_jsr!51
	ldah	$gp, ($26)!gpdisp!53
	lda	$gp, ($gp)!gpdisp!53
L$17:
	.loc 1 125
	ldq	$11, -24($11)												   # 000125
	mov	5, $12
	sll	$12, 30, $12
	subq	$11, $12, $11
	stq	$11, ($10)
	.loc 1 128
L$13:															   # 000128
	.loc 1 130
	ldq	$1, ($9)												   # 000130
	ldq	$10, ($10)
	.loc 1 138
	ldq	$26, ($sp)												   # 000138
	.loc 1 130
	addq	$1, $10, $1												   # 000130
	.loc 1 138
	ldq	$10, 16($sp)												   # 000138
	ldq	$11, 24($sp)
	ldq	$12, 32($sp)
	.loc 1 137
	clr	$0													   # 000137
	.loc 1 130
	stq	$1, ($9)												   # 000130
	.loc 1 138
	ldq	$9, 8($sp)												   # 000138
	lda	$sp, 80($sp)
	.context none
	unop
	ret	($26)
	unop
	unop
	unop
L$44:
	.loc 1 137
	.context none
	clr	$0													   # 000137
	.loc 1 138
	ret	($26)													   # 000138
	.end 	armci_region_register
	unop
	unop
	.loc 1 91
	.loc 1 141
	.globl  armci_region_fixup
	.ent 	armci_region_fixup
	.loc 1 141
armci_region_fixup:
	.context full
	ldah	$gp, ($27)!gpdisp!54
	unop
	lda	$gp, ($gp)!gpdisp!54
	unop
	.frame  $sp, 0, $26
	.prologue 1
L$2:
	.loc 1 142
	sextl	$16, $16												   # 000142
	.loc 1 144
	ldq	$4, armci_me($gp)!literal!55										   # 000144
	.loc 1 143
	ldq	$3, armci_clus_first($gp)!literal!56									   # 000143
	.loc 1 146
	ldq	$6, $$1$info+32($gp)!literal!57										   # 000146
	lda	$6, ($6)!lituse_base!57
	.loc 1 144
	ldl	$4, ($4)!lituse_base!55											   # 000144
	.loc 1 143
	ldl	$3, ($3)!lituse_base!56											   # 000143
	.loc 1 144
	xor	$4, $16, $4												   # 000144
	.loc 1 143
	subl	$16, $3, $3												   # 000143
	.loc 1 144
	beq	$4, L$5													   # 000144
	.loc 1 146
	s8addq	$3, $6, $3												   # 000146
	ldq	$5, ($17)
	ldq	$3, ($3)
	subq	$5, $3, $3
	stq	$3, ($17)
	.loc 1 147
L$5:															   # 000147
	unop
	ret	($26)
	.end 	armci_region_fixup
	.loc 1 141
