using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class SkillUIController : MonoBehaviour
{
    [System.Serializable]
    public class SkillUI
    {
        public Image icon;               // 스킬 아이콘 이미지
        public Image cooldownOverlay;    // 쿨다운 마스크 이미지 (Fill Type: Filled)
        public Image lockOverlay;        // 잠금 표시 이미지
        public float cooldownDuration;   // 해당 스킬 쿨다운 시간

        [HideInInspector] public float lastUsedTime = -999f;  // 마지막 사용 시간 저장
        [HideInInspector] public bool isUnlocked = false;     // 해금 여부
    }

    public SkillUI skillZ; // Z 스킬 UI
    public SkillUI skillX; // X 스킬 UI
    public SkillUI skillC; // C 스킬 UI

    void Start()
    {
        string scene = SceneManager.GetActiveScene().name;

        // 현재 씬에 따라 스킬 해금 상태 설정
        skillZ.isUnlocked = (scene == "Stage1" || scene == "Stage2" || scene == "Stage3");
        skillX.isUnlocked = (scene == "Stage2" || scene == "Stage3");
        skillC.isUnlocked = (scene == "Stage3");

        // 해금 안된 스킬은 쿨다운 마스크 꽉 채우기 (선택사항)
        if (!skillZ.isUnlocked) skillZ.cooldownOverlay.fillAmount = 1f;
        if (!skillX.isUnlocked) skillX.cooldownOverlay.fillAmount = 1f;
        if (!skillC.isUnlocked) skillC.cooldownOverlay.fillAmount = 1f;
    }

    void Update()
    {
        UpdateSkillUI(skillZ);
        UpdateSkillUI(skillX);
        UpdateSkillUI(skillC);
    }

    void UpdateSkillUI(SkillUI skill)
    {
        if (skill.isUnlocked)
        {
            skill.lockOverlay.enabled = false;
            float elapsed = Time.time - skill.lastUsedTime;
            // 쿨다운 진행 비율 (1->0으로 채워짐)
            float fillPercent = Mathf.Clamp01(1f - (elapsed / skill.cooldownDuration));
            skill.cooldownOverlay.fillAmount = fillPercent;
        }
        else
        {
            // 스킬 잠금 상태면 잠금 이미지 표시, 쿨다운 마스크는 꽉 채움
            skill.lockOverlay.enabled = true;
            skill.cooldownOverlay.fillAmount = 1f;
        }
    }

    // 쿨다운 시작 트리거 (키 이름으로 호출)
    public void TriggerCooldown(string key)
    {
        switch (key.ToUpper())
        {
            case "Z":
                if (skillZ.isUnlocked)
                    skillZ.lastUsedTime = Time.time;
                break;
            case "X":
                if (skillX.isUnlocked)
                    skillX.lastUsedTime = Time.time;
                break;
            case "C":
                if (skillC.isUnlocked)
                    skillC.lastUsedTime = Time.time;
                break;
        }
    }
}
