/*
 * [점령 기능 스크립트]
 * 점령 구역에 플레이어가 10초 이상 머무르면 상대방의 체력을 20 감소시키는 기능을 수행하는 스크립트
 */
using UnityEngine;
using Unity.Netcode;


public class CaptureZone : NetworkBehaviour
{
    private float fStayTime = 0.0f;         //플레이어가 점령지에 머무른 시간
    private float fCaptureDuration = 5.0f;  //점령에 필요한 시간

    private bool bCaptured = false; //점령 여부

    private void OnTriggerStay(Collider other)
    {
        NetworkObject netObject = other.GetComponent<NetworkObject>();

        /*
         * 이미 점령된 구역은 무시
         * 플레이어가 아닌 경우 무시
         * 점령은 서버에서만 처리해야하므로 서버에서만 실행
         */
        if (bCaptured) return;
        if (!IsServer) return; 
        if (!other.CompareTag("Player")) return; 
        if (netObject == null || !netObject.IsOwner) return; //네트워크 오브젝트가 없거나 오너가 아닌 경우 무시

        fStayTime += Time.deltaTime; //플레이어가 구역에 머무른 시간 증가시킴

        if (fStayTime >= fCaptureDuration) //점령 시간에 도달했는지 확인
        {
            bCaptured = true; //중복 점령 방지
            Debug.Log("점령 완료!");
            SoundManager.Instance.f_PlaySFX(SoundName.SFX_AreaSecured, 0.7f); //점령 완료 효과음 재생

            GameManager.Instance.f_DamageAllExcept(other.gameObject, 20); //GameManager를 통해 상대방 플레이어의 체력을 감소시킴
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (!other.CompareTag("Player")) return; //플레이어가 아닌 경우 무시

        fStayTime = 0.0f; //타이머 초기화
    }
}
