Point.____Init:
	BeginFunc 0 ;
	*(this + 4) = x ;
	*(this + 8) = y ;
	EndFunc ;
Point.____PrintSelf:
	BeginFunc 20 ;
	_tmp0 = "[" ;
	PushParam _tmp0 ;
	LCall _PrintString ;
	PopParams 4 ;
	_tmp1 = *(this + 4) ;
	PushParam _tmp1 ;
	LCall _PrintInt ;
	PopParams 4 ;
	_tmp2 = ", " ;
	PushParam _tmp2 ;
	LCall _PrintString ;
	PopParams 4 ;
	_tmp3 = *(this + 8) ;
	PushParam _tmp3 ;
	LCall _PrintInt ;
	PopParams 4 ;
	_tmp4 = "]\n" ;
	PushParam _tmp4 ;
	LCall _PrintString ;
	PopParams 4 ;
	EndFunc ;
Point.____PrintBoth:
	BeginFunc 24 ;
	PushParam this ;
	_tmp5 = *(this) ;
	_tmp6 = *(_tmp5 + 4) ;
	ACall _tmp6 ;
	PopParams 4 ;
	PushParam other ;
	_tmp7 = *(other) ;
	_tmp8 = *(_tmp7 + 4) ;
	ACall _tmp8 ;
	PopParams 4 ;
	_tmp9 = *(this + 4) ;
	PushParam _tmp9 ;
	LCall _PrintInt ;
	PopParams 4 ;
	_tmp10 = *(other + 4) ;
	PushParam _tmp10 ;
	LCall _PrintInt ;
	PopParams 4 ;
	EndFunc ;
Point.____equals:
	BeginFunc 28 ;
	_tmp11 = *(this + 4) ;
	_tmp12 = *(other + 4) ;
	_tmp13 = _tmp11 == _tmp12 ;
	_tmp14 = *(this + 8) ;
	_tmp15 = *(other + 8) ;
	_tmp16 = _tmp14 == _tmp15 ;
	_tmp17 = _tmp13 && _tmp16 ;
	Return _tmp17 ;
	EndFunc ;
VTable Point =
	Point.____Init,
	Point.____PrintSelf,
	Point.____PrintBoth,
	Point.____equals,
; 
main:
	BeginFunc 168 ;
	_tmp18 = 8 ;
	_tmp19 = 4 ;
	_tmp20 = _tmp19 + _tmp18 ;
	PushParam _tmp20 ;
	_tmp21 = LCall _Alloc ;
	PopParams 4 ;
	_tmp22 = Point ;
	*(_tmp21) = _tmp22 ;
	p = _tmp21 ;
	_tmp23 = 3 ;
	_tmp24 = 5 ;
	PushParam _tmp24 ;
	PushParam _tmp23 ;
	PushParam p ;
	_tmp25 = *(p) ;
	_tmp26 = *(_tmp25) ;
	ACall _tmp26 ;
	PopParams 12 ;
	PushParam p ;
	_tmp27 = *(p) ;
	_tmp28 = *(_tmp27 + 4) ;
	ACall _tmp28 ;
	PopParams 4 ;
	_tmp29 = 8 ;
	_tmp30 = 4 ;
	_tmp31 = _tmp30 + _tmp29 ;
	PushParam _tmp31 ;
	_tmp32 = LCall _Alloc ;
	PopParams 4 ;
	_tmp33 = Point ;
	*(_tmp32) = _tmp33 ;
	q = _tmp32 ;
	_tmp34 = 5 ;
	_tmp35 = 6 ;
	PushParam _tmp35 ;
	PushParam _tmp34 ;
	PushParam q ;
	_tmp36 = *(q) ;
	_tmp37 = *(_tmp36) ;
	ACall _tmp37 ;
	PopParams 12 ;
	PushParam q ;
	_tmp38 = *(q) ;
	_tmp39 = *(_tmp38 + 4) ;
	ACall _tmp39 ;
	PopParams 4 ;
	PushParam q ;
	PushParam p ;
	_tmp40 = *(p) ;
	_tmp41 = *(_tmp40 + 8) ;
	ACall _tmp41 ;
	PopParams 8 ;
	PushParam q ;
	PushParam p ;
	_tmp42 = *(p) ;
	_tmp43 = *(_tmp42 + 12) ;
	_tmp44 = ACall _tmp43 ;
	PopParams 8 ;
	PushParam _tmp44 ;
	LCall _PrintBool ;
	PopParams 4 ;
	PushParam q ;
	PushParam q ;
	_tmp45 = *(q) ;
	_tmp46 = *(_tmp45 + 12) ;
	_tmp47 = ACall _tmp46 ;
	PopParams 8 ;
	PushParam _tmp47 ;
	LCall _PrintBool ;
	PopParams 4 ;
	EndFunc ;
