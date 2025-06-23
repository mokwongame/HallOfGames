using System.Collections;
using UnityEngine;
using Unity.Netcode;
using UnityEngine.UI;

// 플레이어의 이동과 총알 발사를 담당하는 클래스
public class PlayerController : NetworkBehaviour
{
    private GameObject _bulletPrefabFile;           // 총알 프리팹 저장 변수
    private const string PrefabName = "Bullet";     // 총알 프리팹 리소스명
    private bool _bCanShoot = true;                 // 총알 발사 가능 여부 (쿨다운 적용)
    private const float ShootCooldown = 1f;         // 총알 발사 쿨다운 시간(초)

    [SerializeField]
    private GameObject spawnPoint;                  // 총알이 발사될 위치 (포신 끝 부분 등)


    [SerializeField]
    private UnityEngine.UI.Slider reloadSlider;     // 재장전 슬라이더 연결

    private float _reloadTimer = 0f;

    // 시작 시 총알 프리팹을 리소스에서 불러옴
    void Start()
    {
        _bulletPrefabFile = Resources.Load(PrefabName) as GameObject;

        // 시작 시 슬라이더를 가득 찬 상태로 설정
        if (reloadSlider != null)
        {
            reloadSlider.value = 1f;
        }
    }

    // 매 프레임 호출되는 업데이트 함수
    void Update()
    {
        // 다음 조건 중 하나라도 충족하면 조작 불가:
        // - 오브젝트가 존재하지 않을 때
        // - 게임 매니저가 없거나, 플레이어가 움직일 수 없는 상태
        // - 이 네트워크 오브젝트가 로컬 소유자가 아닐 때 (다른 플레이어 객체 조작 방지)
        if (!gameObject || !BeginGameManager.Instance || !InGameManager.Instance.CanMove || !IsOwner)
        {
            return;
        }

        Movement();                                         // 이동 처리 함수 호출

        // 재장전 타이머가 진행 중이라면 슬라이더 값을 업데이트
        if (!_bCanShoot && reloadSlider != null)
        {
            _reloadTimer -= Time.deltaTime;
            reloadSlider.value = Mathf.Clamp01(_reloadTimer / ShootCooldown);
        }

        // 스페이스바 키를 눌렀고 총알 발사 가능 상태라면 발사 처리
        if (Input.GetKeyDown(KeyCode.Space) && _bCanShoot)
        {
            MakeBulletRpc();                                // 총알 생성 RPC 호출 (서버에 총알 생성 요청)
            StartCoroutine(nameof(ShootCooldownRoutine));   // 쿨다운 코루틴 시작
        }
    }

    // 플레이어 이동 처리 함수
    private void Movement()
    {
        // 수직 입력값에 따라 전진/후진
        float zOff = Input.GetAxis("Vertical") * BeginGameManager.Instance.SpeedTank * Time.deltaTime;
        transform.Translate(0.0f, 0.0f, zOff);

        // 수평 입력값에 따라 좌우 회전
        float angleOff = Input.GetAxis("Horizontal") * BeginGameManager.Instance.RotSpeedTank * Time.deltaTime;
        transform.Rotate(0.0f, angleOff, 0.0f);
    }

    // 총알 생성 요청을 서버에 보내는 RPC (서버에서 실행됨)
    [Rpc(SendTo.Server)]
    private void MakeBulletRpc()
    {
        // 총알 프리팹을 인스턴스화
        GameObject prefab = Instantiate(_bulletPrefabFile);

        // 총알의 발사 위치 설정 (포신 끝 등)
        prefab.GetComponent<BulletNet>().launchPoint = spawnPoint;

        // 총알을 발사한 플레이어 클라이언트 ID 할당 (식별용)
        prefab.GetComponent<BulletNet>().ClientId = OwnerClientId;

        // 총알 위치와 회전을 발사 위치와 동일하게 맞춤
        prefab.transform.position = spawnPoint.transform.position;
        prefab.transform.rotation = spawnPoint.transform.rotation;

        // 네트워크 상에서 총알 오브젝트 스폰(생성)
        prefab.GetComponent<NetworkObject>().Spawn();
    }

    // 총알 발사 쿨다운 코루틴: 일정 시간 동안 발사 금지
    private IEnumerator ShootCooldownRoutine()
    {
        _bCanShoot = false;                                     // 발사 불가 상태로 변경
        _reloadTimer = ShootCooldown;                            // 재장전 타이머 초기화
        yield return new WaitForSeconds(ShootCooldown);         // 쿨다운 시간 대기
        _bCanShoot = true;                                       // 다시 발사 가능 상태로 변경

        // 재장전 완료 시 슬라이더를 가득 채움
        if (reloadSlider != null)
        {
            reloadSlider.value = 1f;
        }
    }
}
