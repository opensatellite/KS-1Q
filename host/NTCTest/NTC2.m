function NTC2(name, R25C, B)

% name='thru';
% R25C = 10e3;
% B = 3478;

side = 0;% 1=�±۵��裬0=�ϱ۵���
Rs = 10e3;%�±۵�����ֵ

ADbits = 12; % ADλ��
lut_scale = 32;

AD_Fs = 2^ADbits-1;
ADrange = 0:lut_scale:2^ADbits;
fname = strcat('ntc_',name,'.h');

f = fopen(fname, 'w');
fprintf(f,'//NTC lookup table\n');
fprintf(f,'//R25C=%d,B=%d\n',R25C,B);
if(side) sname = 'low';
else sname = 'high';
end
fprintf(f,'//Rs=%d,Side=%s\n', Rs, sname);
fprintf(f,'//ADbits=%d,Scale=%d;\n', ADbits, lut_scale);
fprintf(f,'int8_t lut_%s[] = {', name);
Rt=[];
T=[];
for i=1:length(ADrange)
    if(side)
    else
%         Rt(i) = Rs / (AD_Fs / ADrange(i) - 1);
        Rt(i) = Rs * (4095 * 3.3 / 2  / ADrange(i) -1); %#hack
        T(i) = 1 / ( log( Rt(i) / R25C ) / B + 1 / (273.15 + 25)) - 273.15;
%         if(ADrange(i) == 0) T = 127; end
    end
    TT = round(T(i));
    if(TT > 127) TT = 127;
    elseif(TT < -128.0) TT = -128.0;
    end
    fprintf(f, '%d,', TT);
    if(mod(i,32) == 0 && i ~= 0)
        fprintf(f, '\n');
    end
end
fprintf(f,'};\n');
fclose(f);

plot(ADrange, T);
grid on;
xlabel ADC
ylabel Temperature
