main:
	BeginFunc 56 ;
	_tmp0 = 4 ;
	PushParam _tmp0 ;
	LCall _PrintInt ;
	PopParams 4 ;
	_tmp1 = 4 ;
	_tmp2 = 8 ;
	_tmp3 = _tmp1 * _tmp2 ;
	_tmp4 = 1 ;
	_tmp5 = _tmp3 + _tmp4 ;
	PushParam _tmp5 ;
	LCall _PrintInt ;
	PopParams 4 ;
	_tmp6 = "hello" ;
	PushParam _tmp6 ;
	LCall _PrintString ;
	PopParams 4 ;
	_tmp7 = 1 ;
	PushParam _tmp7 ;
	LCall _PrintBool ;
	PopParams 4 ;
	_tmp8 = "cs143\n" ;
	PushParam _tmp8 ;
	LCall _PrintString ;
	PopParams 4 ;
	_tmp9 = 8 ;
	_tmp10 = 3 ;
	_tmp11 = _tmp9 - _tmp10 ;
	_tmp12 = 2 ;
	_tmp13 = _tmp11 - _tmp12 ;
	PushParam _tmp13 ;
	LCall _PrintInt ;
	PopParams 4 ;
	EndFunc ;
