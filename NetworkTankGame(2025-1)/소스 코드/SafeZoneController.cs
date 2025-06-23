/*
 * [SafeZoneController]
 * 생존 가능한 안전 구역(Safe Zone)을 시간에 따라 점차 축소시키는 역할
 * PUBG 스타일의 페이즈 기반 Zone 시스템을 구현함
 */

using System.Collections.Generic;
using UnityEngine;


public class SafeZoneController : MonoBehaviour
{
    [System.Serializable] 
    public class ShrinkPhase //페이즈 클래스 정의
    {
        public float fTimeToStart; //해당 페이즈가 시작될 시간(초)
        public float fTargetScale; //목표 크기(지름 기준)

        public ShrinkPhase(float fTime, float fScale)
        {
            fTimeToStart = fTime;   //페이즈 시작 시간
            fTargetScale = fScale;  //페이즈 목표 크기
        }
    }

    public List<ShrinkPhase> listPhases = new List<ShrinkPhase>(); //페이즈 리스트
    private int nCurrentPhase = 0;          //현재 페이즈 인덱스
    private float fElapsedTime = 0.0f;      //경과 시간

    private Vector3 vOriginalScale = Vector3.zero; //원래 크기 저장용 변수

    void Start()
    {
        vOriginalScale = transform.localScale; //원래 크기 저장

        //페이즈 기본 등록값 (Inspector에서 설정 가능)
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
        fElapsedTime += Time.deltaTime; //경과 시간 업데이트

        if (nCurrentPhase < listPhases.Count) //현재 페이즈가 리스트 범위 내에 있는지 확인
        {
            ShrinkPhase phase = listPhases[nCurrentPhase]; //현재 페이즈 가져오기

            if (fElapsedTime >= phase.fTimeToStart) //현재 페이즈 시작 시간에 도달했는지 확인
            {
                Vector3 vTarget = new Vector3(phase.fTargetScale, transform.localScale.y, phase.fTargetScale);
                transform.localScale = Vector3.Lerp(transform.localScale, vTarget, Time.deltaTime * 1.5f); //크기를 보간법을 사용하여 부드럽게 축소시킴

                if (Vector3.Distance(transform.localScale, vTarget) < 0.1f) //목표 크기에 도달했는지 확인
                {
                    nCurrentPhase++; //다음 페이즈로 이동
                }
            }
        }
    }
}