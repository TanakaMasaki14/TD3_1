#include "OBJShaderHeader.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{

	//�E�A���A���̕��������������C�g
	float3 lightdir = float3(1.0f, -1.0f, 1.0f);
	lightdir = normalize(lightdir);
	//���C�g�̐F
	float3 lightcolor = float3(1.0f, 1.0f, 1.0f);

	//�����ˌ�
	float3 ambient = m_ambient;
	//�g�U���ˌ�
	float3 diffuse = dot(-lightdir, normal) * m_diffuse;

	//���_���W
	const float3 eye = float3(0.0f, 0.0f, -20.0f);
	//����x
	const float shininess = 4.0f;
	//���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(eye - pos.xyz);
	//���ˌ��x�N�g��
	float3 reflect = normalize(lightdir + 2 * dot(-lightdir, normal) * normal);
	//���ʔ��ˌ�
	float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	VSOutput output; // �s�N�Z���V�F�[�_�[�ɓn���l
	output.svpos = mul(mat, pos);

	//�����o�[�g���˂̌v�Z
	output.color.rgb = (ambient + diffuse + specular) * lightcolor;
	output.color.a = m_alpha;
	output.uv = uv;
	return output;
}