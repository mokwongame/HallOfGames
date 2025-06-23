/*
 * [SafeZoneController]
 * ���� ������ ���� ����(Safe Zone)�� �ð��� ���� ���� ��ҽ�Ű�� ����
 * PUBG ��Ÿ���� ������ ��� Zone �ý����� ������
 */

using System.Collections.Generic;
using UnityEngine;


public class SafeZoneController : MonoBehaviour
{
    [System.Serializable] 
    public class ShrinkPhase //������ Ŭ���� ����
    {
        public float fTimeToStart; //�ش� ����� ���۵� �ð�(��)
        public float fTargetScale; //��ǥ ũ��(���� ����)

        public ShrinkPhase(float fTime, float fScale)
        {
            fTimeToStart = fTime;   //������ ���� �ð�
            fTargetScale = fScale;  //������ ��ǥ ũ��
        }
    }

    public List<ShrinkPhase> listPhases = new List<ShrinkPhase>(); //������ ����Ʈ
    private int nCurrentPhase = 0;          //���� ������ �ε���
    private float fElapsedTime = 0.0f;      //��� �ð�

    private Vector3 vOriginalScale = Vector3.zero; //���� ũ�� ����� ����

    void Start()
    {
        vOriginalScale = transform.localScale; //���� ũ�� ����

        //������ �⺻ ��ϰ� (Inspector���� ���� ����)
        if (listPhases.Count == 0)
        {
            listPhases.Add(new ShrinkPhase(0.1f, 250.0f));
            listPhases.Add(new ShrinkPhase(20.0f, 150.0f));
            listPhases.Add(new ShrinkPhase(30.0f, 100.0f));
            listPhases.Add(new ShrinkPhase(40.0f, 50.0f));
            listPhases.Add(new ShrinkPhase(60.0f, 20.0f));
        }
    }

    void Update()
    {
        fElapsedTime += Time.deltaTime; //��� �ð� ������Ʈ

        if (nCurrentPhase < listPhases.Count) //���� ����� ����Ʈ ���� ���� �ִ��� Ȯ��
        {
            ShrinkPhase phase = listPhases[nCurrentPhase]; //���� ������ ��������

            if (fElapsedTime >= phase.fTimeToStart) //���� ������ ���� �ð��� �����ߴ��� Ȯ��
            {
                Vector3 vTarget = new Vector3(phase.fTargetScale, transform.localScale.y, phase.fTargetScale);
                transform.localScale = Vector3.Lerp(transform.localScale, vTarget, Time.deltaTime * 1.5f); //ũ�⸦ �������� ����Ͽ� �ε巴�� ��ҽ�Ŵ

                if (Vector3.Distance(transform.localScale, vTarget) < 0.1f) //��ǥ ũ�⿡ �����ߴ��� Ȯ��
                {
                    nCurrentPhase++; //���� ������� �̵�
                }
            }
        }
    }
}