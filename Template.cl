__kernel void Add(__global char pA[][30], __global int pB[],__global int pBVer[],__global int *c2)
{
	int id = get_global_id(0);int i=0;
	while (pA[id][i] != '\n'&&pA[id][i]!='Ì')
		{
		if (pA[id][i] == '2')
			i++;
			else if (pA[id][i] == '1')
			{
				int onec = 0;
				while (pA[id][i] == '1')
				{
					onec++; i++;
				}
				if (id>=*c2)
				pBVer[onec]++;
				else
				pB[onec]++; 
				
			}

			else
			{
				int zeroc = 0;
				while (pA[id][i] == '0')
				{
					zeroc++; i++;
				}
				if (id>=*c2)
				pBVer[zeroc]++;
				else
				pB[zeroc]++; 
			}
		}
	}
