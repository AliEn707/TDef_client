readlines.each do|l| 
	a=eval(l.gsub(",\r\n","").gsub("{","[").gsub("}","]").gsub(/\/\*[\w[ ]]+\*\//,""))
	c=a[0]
	s=a[1][3]
	e=a[1][4]
	printf "{%s,\n\t{0,0,0,0x%x/*%d*/,0x%x/*%d*/,0}},\t//%s\n",a[0].downcase.gsub(/[- ]/,'_').inspect,s,s,e,e,c
end