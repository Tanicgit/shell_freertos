


/*
字符串分割 不可重入
返回的_stringList 用完后要调用 FreeStringList释放
*/
_stringList *mtStrDiv(char *a, char *b)
{
	char *c,*d[LIST_MAX_NUM];
	_stringList *list;
	
	uint8_t i=0;
	c = strtok(a,b);
	while(c!=NULL)
	{
		d[i] = malloc(strlen(c));	
		if(d[i]==NULL)return NULL;
		strcpy(d[i],c);
		c = strtok(NULL,b);
		i++;
		if(i==LIST_MAX_NUM)break;
	}
	if(i==0)return NULL;
	list=malloc(sizeof(_stringList));
	if(list==NULL)return NULL;
	list->num=i;
	memcpy(list->d,d,sizeof(char*)*i);
	return list;
}


void FreeStringList(_stringList *list)
{
	uint8_t i=0;
	for(i=0;i<list->num;i++)
	free(list->d[i]);
	free(list);
}
